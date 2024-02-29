// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"


class UPawnSensingComponent;
class USAttributeComponent;
class USWorldUserWidget;

UCLASS()
class LEARNINGCPPUE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

protected:

	USWorldUserWidget* ActiveHealthBar;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass; // MDJ: Lecture 14: add onscreen healthbar

	virtual void PostInitializeComponents() override;

	void SetTargetActor(AActor* NewTarget);

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

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;
};
