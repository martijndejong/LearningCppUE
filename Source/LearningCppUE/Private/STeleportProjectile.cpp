// Fill out your copyright notice in the Description page of Project Settings.


#include "STeleportProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"


ASTeleportProjectile::ASTeleportProjectile()
{
	MovementComp->InitialSpeed = 3000.0f;
}

// Called when the game starts or when spawned
void ASTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();

	float ExplodeDelay = 0.2f;
	GetWorldTimerManager().SetTimer(TimerHandle_Explode, this, &ASTeleportProjectile::Explode_TimeElapsed, ExplodeDelay);
}

void ASTeleportProjectile::Explode_TimeElapsed()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeParticleSystem, this->GetTransform());
	MovementComp->StopMovementImmediately();

	float TeleportDelay = 0.2f;
	GetWorldTimerManager().SetTimer(TimerHandle_Teleport, this, &ASTeleportProjectile::Teleport_TimeElapsed, TeleportDelay);
}

void ASTeleportProjectile::Teleport_TimeElapsed()
{
	AActor* MyInstigator = GetInstigator();
	MyInstigator->TeleportTo(this->GetActorLocation(), this->GetActorRotation());

	this->Destroy();
}
