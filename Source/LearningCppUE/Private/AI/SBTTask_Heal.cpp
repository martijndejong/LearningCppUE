// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_Heal.h"

#include "AIController.h"
#include "GameFramework/Character.h"

#include "SAttributeComponent.h"

EBTNodeResult::Type USBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (ensure(MyController))
	{
		// MDJ: Try to get Pawn controlled by AI controller. If none, then return task failed
		ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());
		if (MyPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(MyPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (ensure(AttributeComp))
		{
			bool HasHealed = AttributeComp->ApplyHealthChange(AttributeComp->GetHealthMax());

			return HasHealed ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
		}
	}
	return EBTNodeResult::Failed;
}
