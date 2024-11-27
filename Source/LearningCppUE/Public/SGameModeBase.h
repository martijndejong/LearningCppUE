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

	// MDJ: Lecture 15.2 -- adding Timer Delegate with function
	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);


	// Assignment 5:
	// Read/write access as we could change this as our difficulty increases via Blueprint
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	int32 CreditsPerKill;

	/* Distance required between power-up spawn locations */
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	float RequiredPowerupDistance;

	/* Amount of powerups to spawn during match start */
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	int32 DesiredPowerupCount;

	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	TObjectPtr<UEnvQuery> PowerupSpawnQuery;

	/* All power-up classes used to spawn with EQS at match start */
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	TArray<TSubclassOf<AActor>> PowerupClasses;

	void OnPowerupSpawnQueryCompleted(TSharedPtr<FEnvQueryResult> Result);


public: 
	ASGameModeBase();

	// GameModeBase does not have BeginPlay - it actualy calls BeginPlay on all actors -- instead it has StartPlay
	virtual void StartPlay() override;


	UFUNCTION(Exec)
	void KillAll();

	// MDJ: Lecture 15.2
	// MDJ: Make virtual so that it can be overwritten in child classes of gamemode
	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);
	
};
