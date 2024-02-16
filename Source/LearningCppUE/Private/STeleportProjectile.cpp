// Fill out your copyright notice in the Description page of Project Settings.


#include "STeleportProjectile.h"
// Needed to import this to use MovementComp from parent class
#include "GameFramework/ProjectileMovementComponent.h"

// Needed these imports for UGameplayStatics::SpawnEmitterAtLocation
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"


ASTeleportProjectile::ASTeleportProjectile()
{	
	// Parent class constructor automatically called, does not need explicit super
	MovementComp->InitialSpeed = 3000.0f;
}

// Called when the game starts or when spawned
void ASTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();

	// Set timer to call Explode_TimeElapsed function
	float ExplodeDelay = 0.2f;

	// Note that here you just call 'Explode', not 'Explode_Implementation'
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedExplode, this, &ASTeleportProjectile::Explode, ExplodeDelay);
}

void ASTeleportProjectile::Explode_Implementation()
{
	// clear timer if the Exlode was already through another source like OnActorHit (base class logic)
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedExplode);

	// Spawn particle Emitter at location
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactVFX, this->GetTransform());
	
	// Stop movement comp so that teleport location does not shift after emitter
	// MDJ: it would have been more neat to add a function to the parent class e.g. 'StopProjectile' and let that do below code 
	//		for readability and maintainability it is not nice to make direct reference to attributes of parent class, functions encapsulate it more neatly
	MovementComp->StopMovementImmediately();

	// Set new timer to call Teleport_TimeElapsed
	float TeleportDelay = 0.2f;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &ASTeleportProjectile::Teleport_TimeElapsed, TeleportDelay);

	// Skip base implementation of Explode as it will destroy actor (we need it to stay alive a little longer to finish the 2nd timer)
	//Super::Explode_Implementation();
}

void ASTeleportProjectile::Teleport_TimeElapsed()
{
	// Teleport to location of explosion
	AActor* MyInstigator = GetInstigator();
	if (ensure(MyInstigator))
	{
		MyInstigator->TeleportTo(this->GetActorLocation(), MyInstigator->GetActorRotation());
	}
	
	this->Destroy();
}
