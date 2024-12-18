// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SProjectileBase.h"

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_ProjectileAttack.generated.h"


class UAnimMontage;
class UParticleSystem;

/**
 * 
 */
UCLASS()
class LEARNINGCPPUE_API USAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()
	

protected:
	// MDJ: This makes it so that from the BP you can set a 'ProjectileClass' (of type <AActor>) which will be used by SpawnActor in ASCharacter::PrimaryAttack()
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<ASProjectileBase> ProjectileClass;
	
	// MDJ: VisibleAnywhere = read-only, still useful to view in-editor and enforce a convention
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackAnimDelay;

	// MDJ: Setting a category is good practice to categorize in the editor (does not affect any way the code runs, solely for how it's shown in the editor)
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	// Particle system played during attack animation
	UPROPERTY(EditAnywhere, Category = "Attack")
	UParticleSystem* CastingEffect;


	// This is the function that is called once timer is elapsed - basically it just spawns the projectile once casting time is done
	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);


public: 

	// Override parent functionality of StartAction event
	virtual void StartAction_Implementation(AActor* Instigator) override; 


	// Constructor
	USAction_ProjectileAttack();
	
};
