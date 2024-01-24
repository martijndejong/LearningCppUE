// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SExplosiveBarrel.generated.h"

class UStaticMeshComponent;
class URadialForceComponent;

UCLASS()
class LEARNINGCPPUE_API ASExplosiveBarrel : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASExplosiveBarrel();


	// This protected section added entirely by MDJ
protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere)
	URadialForceComponent* RadialForce;

	// This must also be added to make PostInitializeComponents() work
	virtual void PostInitializeComponents() override;

	// Must be marked with ufunction in order to be able to 'bind' the event
	UFUNCTION()
	// Had to add this 'signature' to the function for it to work -- it is what OnComponentHit passes to the function on default
	void ExplosionFunction(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};