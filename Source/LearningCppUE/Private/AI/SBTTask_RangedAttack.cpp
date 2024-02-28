// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_RangedAttack.h"

#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SAttributeComponent.h"


USBTTask_RangedAttack::USBTTask_RangedAttack()
{
	MaxBulletSpread = 2.0f; // default value (degrees)
}


EBTNodeResult::Type USBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

		// MDJ: From Pawn, get socket location of "muzzle_01"
		FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");

		// MDJ: Get target actor from BlackboardComponent (this was set using "GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);" in SAIController.cpp -- but will be moved to Pawn sensing)
		// MDJ: if no current target actor (non sensed) then early out and return failed
		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		if (TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		// MDJ: Check if target actor is alive, if not, return failed
		// MDJ: using our custom static function to check if actor if alive without having to get their AttributeComponent instance
		// MDJ: Would be bad practice to also set TargetActor to nullptr here, because that is not the responsibility of this Task
		if (!USAttributeComponent::IsActorAlive(TargetActor))
		{
			return EBTNodeResult::Failed;
		}

		// MDJ: Calculate direction in which ranged attack will be fired
		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = Direction.Rotation();
		// Add random bullet spread to spawn rotator
		MuzzleRotation.Pitch += FMath::RandRange(0.0f, MaxBulletSpread); // MDJ: min pitch spread to 0 so that he does not shoot into floor
		MuzzleRotation.Yaw += FMath::RandRange(-MaxBulletSpread, MaxBulletSpread);

		// MDJ: Set up spawn parameters -- set collision handling to always spawn
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Params.Instigator = MyPawn;

		// MDJ: Spawn projectile actor and return succeeded if successfully spawned, else return failed
		AActor* NewProj = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, Params);

		return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}

