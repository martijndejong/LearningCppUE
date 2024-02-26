// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SBTService_CheckLowHealth.generated.h"

/**
 * 
 */
UCLASS()
class LEARNINGCPPUE_API USBTService_CheckLowHealth : public UBTService
{
	GENERATED_BODY()

protected:
	// MDJ: Assignment 4 -- below copied from SBTService_GetAttackRange:
	// 
	// MDJ: FBlackboardKeySelector "will handle the magical dropdown for us, with all possible blackboard values" -- instead of FName, turns to FName using .SelectedKeyName
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector LowHealthKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
