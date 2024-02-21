// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
// #include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ASAICharacter::ASAICharacter()
{
	// MDJ: Add Pawn Sensing Component to our AI Character
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	// MDJ: The movement of the AI Pawn was jerky - to make for smooth turning, we fiddled with below settings in BP
	//		After finding the correct settings, you can 'bake' them into your C++ class (as done below)
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	bUseControllerRotationYaw = false;
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
