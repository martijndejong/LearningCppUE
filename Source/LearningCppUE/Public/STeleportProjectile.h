// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SMagicProjectile.h"
#include "STeleportProjectile.generated.h"

class UParticleSystem;


UCLASS()
class LEARNINGCPPUE_API ASTeleportProjectile : public ASMagicProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASTeleportProjectile();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UParticleSystem* ExplodeParticleSystem;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// MDJ: Adding timer to time teleport
	FTimerHandle TimerHandle_Explode;
	FTimerHandle TimerHandle_Teleport;

	void Explode_TimeElapsed();
	void Teleport_TimeElapsed();

};
