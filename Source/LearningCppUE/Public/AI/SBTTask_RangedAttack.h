// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SBTTask_RangedAttack.generated.h"

/**
 * 
 */
UCLASS()
class LEARNINGCPPUE_API USBTTask_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	// MDJ: Create UPROPERTY that determines what projectile class will be spawned by AI Pawn in BehaviorTreeTask_RangedAttack
	// MDJ: Had to set to 'EditAnywhere' to properly expose to our AI window, 'EditDefaultOnly' would NOT make it visible in editor
	UPROPERTY(EditAnywhere, Category = "AI")
	TSubclassOf<AActor> ProjectileClass;
};
