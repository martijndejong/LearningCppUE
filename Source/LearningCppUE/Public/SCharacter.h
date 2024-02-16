// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USInteractionComponent;
class UAnimMontage;
class USAttributeComponent;

UCLASS()
class LEARNINGCPPUE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()
	// MDJ: GENERATED_BODY is 'Boilerplate', must remain first line of class

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	// MDJ: this is for adding the interaction component to the character
	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;

	// MDJ: Adding our own AttributeComponent to the SCharacter class
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components") // BlueprintReadOnly to get access to it on the BluePrint graph
	USAttributeComponent* AttributeComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// MDJ: Must declare void MoveForward function in .h before can be created in .cpp
	void MoveForward(float Value);
	void MoveRight(float Value);

	// MDJ: Must declare function to be referenced by BindAction -- no value has to be passed, "it will just triggers and that's it"
	void PrimaryAttack();
	void BlackholeAttack();
	void Dash();

	// MDJ: Declare PrimaryInteract function to be called when interact button is pressed
	void PrimaryInteract();

	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);


	// MDJ: Setting a category is good practice to categorize in the editor (does not affect any way the code runs, solely for how it's shown in the editor)
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	// MDJ: This makes it so that from the BP you can set a 'ProjectileClass' (of type <AActor>) which will be used by SpawnActor in ASCharacter::PrimaryAttack()
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;
	// MDJ: do the same for black hole and dash
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> BlackholeProjectileClass;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> DashProjectileClass;


	// MDJ: Adding timer to time to animation with attack
	FTimerHandle TimerHandle_PrimaryAttack;
	void PrimaryAttack_TimeElapsed();

	FTimerHandle TimerHandle_BlackholeAttack;
	void BlackholeAttack_TimeElapsed();
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
