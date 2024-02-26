// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "EnvironmentQuery/EnvQueryManager.h"

#include "AI/SAICharacter.h"
#include "SAttributeComponent.h"
#include "EngineUtils.h" // MDJ: for TActorIterator


ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	// Continuous timer to spawn in more bots.
	// Actual amount of bots and whether it's allowed to spawn determined by spawn logic later in the chain...
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void ASGameModeBase::SpawnBotTimerElapsed()
{
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	// Ensure added, because could easily go wrong with user error if no SpawnBotQuery is set in editor
	if (ensure(QueryInstance))
	{
		// MDJ: Two notes here:
		//		1) The RunEQSQuery runs asynchronous (or with max time slice) and thus does not have instant result (can take multiple dts)
		//		2) EQS is dated, thus binding an event is a little different. We need to bind to ->GetOnQueryFinishedEvent()
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
	}
}

// MDJ: So this will run a few frames after we have started our EQS query
void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	// MDJ: If query has any other status than success, return (and write warning to log so that it does not fail silently)
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"))
		return;
	}

	


	// MDJ: Limit the number of AI spawned in the level at once using 'TActorIterator' -- "is like a better version of 'GetAllActorsOfClass'
	int32 NrOfAliveBots = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;


		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(Bot->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp && AttributeComp->IsAlive())
		{
			NrOfAliveBots++;
		}
	}
	float MaxBotCount = 10.0f;

	// MDJ: Using a UCurveFloat to progressively increase difficulty -- assigned asset made in UE editor (right click > miscellaneous > curve)
	if (DifficultyFloat)
	{
		MaxBotCount = DifficultyFloat->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (NrOfAliveBots >= MaxBotCount)
	{
		return;
	}
	// end of limiting spawn count




	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	// MDJ: Check if returned locations array in fact has elements
	if (Locations.Num() > 0) // MDJ: Could also use Locations.IsValidIndex(0)
	{
		// MDJ: Spawn minion class at first array element
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
	}
}
