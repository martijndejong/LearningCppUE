// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "STeleportProjectile.generated.h"

class UParticleSystem;


UCLASS()
class LEARNINGCPPUE_API ASTeleportProjectile : public ASProjectileBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASTeleportProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// MDJ: Adding timer to time teleport
	FTimerHandle TimerHandle_DelayedExplode;
	FTimerHandle TimerHandle_DelayedTeleport;

	// Base class using BlueprintNativeEvent. We must override the _Implementation, not the Explode()
	virtual void Explode_Implementation() override;
	void Teleport_TimeElapsed();

};
