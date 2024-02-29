// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
// #include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"

// Assignment 4
#include "SAttributeComponent.h"
#include "SWorldUserWidget.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
	// MDJ: Add Pawn Sensing Component to our AI Character
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	// MDJ: Assignment 4: Add our own Attribute Component to the AI Character
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	// MDJ: The movement of the AI Pawn was jerky - to make for smooth turning, we fiddled with below settings in BP
	//		After finding the correct settings, you can 'bake' them into your C++ class (as done below)
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	bUseControllerRotationYaw = false;

	// MDJ: AI also did not auto possess when Spawned (only when placed) thus change this in BP and now baked into C++ constructor:
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	TimeToHitParamName = "TimeToHit";
}

// MDJ: Do binding of events in PostInitializeComponents() to prevent issues that were faced when doing this in the constructor
void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();


	// MDJ: Bind the OnSeePawn event of the PawnSensingComp to run our ASAICharacter::OnPawnSeen function
	// 
	// MDJ: To find the name of the delegate, go to the <UPawnSensingComponent> .h (crtl click it) 
	//		At the top you will see DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FSeePawnDelegate, APawn*, Pawn ); 
	//		Search the 'FSeePawnDelegate' and we see it executes delegate 'OnSeePawn'
	//		To see an implementation of this from scratch, look at the SAttributeComponent .h (for delegate declaration) and .cpp (for broadcasting)
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);


	// MDJ: Assignment 4 
	// MDJ: binding function to AttributeComp 'OnHealthChanged' event -- this function checks if player died and disables input
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChangedFunc);
}


// MDJ: BELOW FUNCTION COPIED DIRECTLY FROM ASCharacter::OnHealthChangedFunc
void ASAICharacter::OnHealthChangedFunc(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		// MDJ: Lecture 14: add onscreen health bar when hit
		if (ActiveHealthBar == nullptr)
		{
			// MDJ: ActiveHealthBar NOT a local variable, so that it can be referenced, i.e., only create widget if ActiveHealthBar is nullptr
			//		this is done by adding "USWorldUserWidget* ActiveHealthBar;" in the .h file
			ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			// MDJ: Only add to viewport if succesfully created
			if (ActiveHealthBar)
			{
				// MDJ: Must set AttachedActor BEFORE AddToViewPort(), because add to viewport will run 'Event Construct' in BP
				//		Go look at MinionHealth_Widget BP Graph 'Event Construct' and you will see that it references AttachedActor
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}
		


		// MDJ: hit flash effect -- same as 'TargetDummy'
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);

		// MDJ: Set TargetActor to the instigator who attacked the AI character (sensing)
		if (InstigatorActor != this) // MDJ: Only do this if the instigator is not ourselves
		{
			SetTargetActor(InstigatorActor);
		}

		if (NewHealth <= 0.0f)
		{
			// Stop BT
			AAIController* AIController = Cast<AAIController>(GetController());
			if (AIController)
			{
				AIController->GetBrainComponent()->StopLogic("Killed"); // MDJ: "Killed" here is only for debugging purposes, does not influence gameplay
			}

			// Ragdoll
			// MDJ: Need to use GetMesh() instead of accessing mesh directly because they have marked the Mesh in character class as private
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll"); // MDJ: Change Collision profile because original one only does Query (no physics) so sinks through ground

			// Set lifespan (destroy actor)
			SetLifeSpan(10.0f);
		}
	}

	// MDJ: DEBUG ASSIGNMENT 4:
	if (NewHealth <= 50.0f)
	{
		DrawDebugString(GetWorld(), GetActorLocation(), "LOW HEALTH", nullptr, FColor::Red, 4.0f, true);
	}
}

// MDJ: This functions sets the "TargetActor" in the BlackboardComponent of the AI to the sensed (seen) Pawn
void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}
}

// MDJ: This function is bound to OnPawnSeen event and then calls SetTargetActor to set new TargetActor
void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
	DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
}