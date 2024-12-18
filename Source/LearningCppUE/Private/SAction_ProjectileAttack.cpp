// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_ProjectileAttack.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	HandSocketName = "Muzzle_01";
	AttackAnimDelay = 0.2f;
}

void USAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	// Cast our instigator to ACharacter type so that we can call play anim montage on it
	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (ensure(Character))
	{
		Character->PlayAnimMontage(AttackAnim);

		UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

		// Set timer to actually fire attack
		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;  // You use a delegate if the function you are calling requires arguments, like AttackDelay_Elapsed(Character).
		Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);  // 'Character' here passes Character to our AttackDelay_Elapsed function

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
	}
	

}

void USAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (ensureAlways(ProjectileClass))
	{
		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);


		/* Line trace to calculate spawn direction */
		// Set collision shape
		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		// Ignore player
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorCharacter);

		// Set collision object query parameters
		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		// Trace start and end
		FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();  // GetPawnViewLocation overridden in our ASCharacter class to return camera viewpoint instead of pawn viewpoint
		FVector TraceEnd = TraceStart + (InstigatorCharacter->GetControlRotation().Vector() * 5000);

		// Execute trace
		FHitResult Hit;
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
		{
			// Overwrite trace end with impact point in world
			TraceEnd = Hit.ImpactPoint;
		}


		/* Calculate spawn direction and spawn */
		// Find new direction/rotation from Hand pointing to trace impact point in world
		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();

		// Set up Spawn Transform
		FTransform SpawnTM = FTransform(ProjRotation, HandLocation);

		// Always spawn and set instigator
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;

		// Spawn projectile
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}

	StopAction(InstigatorCharacter);
}
