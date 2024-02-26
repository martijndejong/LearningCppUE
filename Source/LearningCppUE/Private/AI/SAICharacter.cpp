// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
// #include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"

// Assignment 4
#include "SAttributeComponent.h"

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

// MDJ: This functions sets the "TargetActor" in the BlackboardComponent of the AI to the sensed (seen) Pawn
void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		UBlackboardComponent* BBComp = AIController->GetBlackboardComponent();

		BBComp->SetValueAsObject("TargetActor", Pawn);

		DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
	}
}


// MDJ: BELOW FUNCTION COPIED DIRECTLY FROM ASCharacter::OnHealthChangedFunc
void ASAICharacter::OnHealthChangedFunc(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		// MDJ: hit flash effect -- same as 'TargetDummy'
		GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}

	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		AAIController* AIController = Cast<AAIController>(GetController());
		if (ensure(AIController))
		{
			Destroy();
		}
	}

	// MDJ: DEBUG ASSIGNMENT 4:
	if (NewHealth <= 50.0f)
	{
		DrawDebugString(GetWorld(), GetActorLocation(), "LOW HEALTH", nullptr, FColor::Red, 4.0f, true);
	}
}