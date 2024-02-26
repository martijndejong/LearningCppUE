// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"


class UPawnSensingComponent;
class USAttributeComponent;

UCLASS()
class LEARNINGCPPUE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:
	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp; // MDJ: there is also AIPerception -- PawnSensing is the predecessor of AIPerception. AIPerception is 'better' more options
											// PawnSensing is the most basic version and easier to set up. Since we only need basic functionality, it is more applicable for us

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);


	// MDJ: Assignment 4: 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComp;

	UFUNCTION()
	void OnHealthChangedFunc(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
};
