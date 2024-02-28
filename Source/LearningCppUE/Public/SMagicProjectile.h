// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SMagicProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

UCLASS()
class LEARNINGCPPUE_API ASMagicProjectile : public ASProjectileBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASMagicProjectile();

protected:
	// Must be marked up as UFUNCTION otherwise we cannot properly bind the event
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Moved to SProjectileBase.h
	//// MDJ: 'UPROPERTY(VisibleAnywhere)' makes it visible and assignable from the editor
	//// MDJ: had to add BlueprintReadOnly to also be able to use it in the BP event graph
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//USphereComponent* SphereComp;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//UProjectileMovementComponent* MovementComp;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//UParticleSystemComponent* EffectComp;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float Damage;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
