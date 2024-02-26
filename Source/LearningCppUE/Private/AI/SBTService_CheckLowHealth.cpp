// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckLowHealth.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "SAttributeComponent.h"

void USBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();

	if (ensure(MyController) && ensure(BlackboardComp))
	{
		APawn* MyPawn = MyController->GetPawn();
		if (MyPawn)
		{
			USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(MyPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
			if (ensure(AttributeComp))
			{
				bool HasLowHealth = AttributeComp->GetHealth() < 50.0f;
				BlackboardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, HasLowHealth);
			}
		}
	}
}
