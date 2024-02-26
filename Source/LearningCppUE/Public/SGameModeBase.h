// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h" // MDJ: Must include this to work
#include "SGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;

/**
 * 
 */
UCLASS()
class LEARNINGCPPUE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()


protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyFloat;

	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;

	void SpawnBotTimerElapsed();

	UFUNCTION() // MDJ: Initially forgot to put UFUNCTION() and debugger said: 
				//		"Unable to bind delegate to 'OnQueryCompleted' (function might not be marked as a UFUNCTION or object may be pending kill)"
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

public: 
	ASGameModeBase();

	// GameModeBase does not have BeginPlay - it actualy calls BeginPlay on all actors -- instead it has StartPlay
	virtual void StartPlay() override;
	
};
