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
class USActionComponent;

UCLASS()
class LEARNINGCPPUE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()
	// MDJ: GENERATED_BODY is 'Boilerplate', must remain first line of class

public:
	// Sets default values for this character's properties
	ASCharacter();

	// Override GetPawnViewLocation to make it return camera viewpoint instead of Pawn viewpoint
	virtual FVector GetPawnViewLocation() const override;

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

	// MDJ: Adding our own ActionComponent to the SCharacter class
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components") // BlueprintReadOnly to get access to it on the BluePrint graph
	USActionComponent* ActionComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Sprint actions (added in lecture 16.2)
	void SprintStart();

	void SprintStop();

	// MDJ: Must declare void MoveForward function in .h before can be created in .cpp
	void MoveForward(float Value);
	void MoveRight(float Value);

	// MDJ: Must declare function to be referenced by BindAction -- no value has to be passed, "it will just triggers and that's it"
	void PrimaryAttack();
	void BlackholeAttack();
	void Dash();

	// MDJ: Declare PrimaryInteract function to be called when interact button is pressed
	void PrimaryInteract();
	
	/* LECTURE 16 - CONVERTING THE PROJECTILE ATTACKS TO "ACTIONS" - Standardizing and moving logic to Action_ProjectileAttack
	COPIED TO SAction_ProjectileAttack.h

	// MDJ: VisibleAnywhere = read-only, still useful to view in-editor and enforce a convention
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName;

	// MDJ: Setting a category is good practice to categorize in the editor (does not affect any way the code runs, solely for how it's shown in the editor)
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackAnimDelay;

	// MDJ: This makes it so that from the BP you can set a 'ProjectileClass' (of type <AActor>) which will be used by SpawnActor in ASCharacter::PrimaryAttack()
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;
	*/

	// MDJ: do the same for black hole and dash
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> BlackholeProjectileClass;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> DashProjectileClass;


	/* LECTURE 16 - CONVERTING THE PROJECTILE ATTACKS TO "ACTIONS" - Standardizing and moving logic to Action_ProjectileAttack
	void SpawnProjectile(TSubclassOf<AActor> ClassToSpawn);
	
	// MDJ: Adding timer to time to animation with attack
	FTimerHandle TimerHandle_PrimaryAttack;
	void PrimaryAttack_TimeElapsed();

	FTimerHandle TimerHandle_BlackholeAttack;
	void BlackholeAttack_TimeElapsed();
	*/


	// MDJ: adding this in lecture 9 for stopping input on death
	UFUNCTION()
	void OnHealthChangedFunc(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
	// MDJ: use PostInitializeComponents to bind the HealthChanged event to above function -- this is better than using the constructor for event binding
	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// MDJ: Lecture 14.4 exec function (console command) -- these will work automatically if in:
	//		1 Character you're controlling
	//		2 Player Controller
	//		3 GameMode
	//		4 Cheat manager
	UFUNCTION(Exec)
	void HealSelf(float Amount=100);
};
