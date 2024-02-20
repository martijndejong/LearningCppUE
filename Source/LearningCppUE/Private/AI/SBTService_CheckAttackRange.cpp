// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckAttackRange.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"


// MDJ: When Tom tries to build this he gets error "unresolved external symbol"-- he fixes this by including module
//		Explained in lecture 10.3
void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Goal: Check distance between ai pawn and target actor
	// MDJ: below we use '.' instead of '->', because we passing a object REFERENCE instead of POINTER -- AI module breaks some of the conventions in that regard
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp))
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor")); // MDJ: Returns UObject, cast to Actor so that we can use GetActorLocation
		if (TargetActor) // MDJ: This time not with ensure() because it is expected that this will also be null many times
		{
			AAIController* AIController = OwnerComp.GetAIOwner(); // MDJ: OwnerComp (UBehaviorTreeComponent) has .GetAIOwner() to return owning AI controller
			if (ensure(AIController))
			{
				APawn* AIPawn = AIController->GetPawn();
				if (ensure(AIPawn)) // MDJ: ensure added here because "if AIPawn is null, but we're still running our BehaviorTree, then something is wrong
				{
					// MDJ: Check if within Attack Range
					float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
					bool bWithinRange = DistanceTo < 1000.0f;

					// MDJ: Check line of sight -- AI Controller has built in logic for this, so we do not have to set up a line trace
					// MDJ: Only do line of sight check if within range, to save resources
					bool bHasLOS = false;
					if (bWithinRange)
					{
						bHasLOS = AIController->LineOfSightTo(TargetActor);
					}


					// MDJ: Now that we have calculated the booleans, we put them back into the Blackboard
					// MDJ: 'AttackRangeKey' is set through the Blacboard in Editor (therefor set as 'FBlackboardKeySelector' type in the .h)
					//		This makes it so that we do not have to hardcode the FName and match it in the Blackboard hardcoded name
					//		Instead, we now see 'Attack Range Key' appear in the editor when selecting the Service, and we select the boolean we created in the Blackboard in the Editor
					BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, (bWithinRange && bHasLOS));
				}
			}
		}
	}
}
