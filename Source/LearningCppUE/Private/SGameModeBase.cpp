// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "EnvironmentQuery/EnvQueryManager.h"

#include "AI/SAICharacter.h"
#include "SAttributeComponent.h"
#include "EngineUtils.h" // MDJ: for TActorIterator
#include "SCharacter.h"
#include "SPlayerState.h"




// MDJ: Lecture 15.3 Console Variables
// <variable type>(Command name, default value, tooltip, mark - using ECVF_Cheat makes it that it won't be included in actual build
// the su. in command name is for Stanford University, i.e., use whatever prefix makes sense for your project to separate your own ConsoleVariables
static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);



ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.0f;
	CreditsPerKill = 20;

	DesiredPowerupCount = 10;
	RequiredPowerupDistance = 2000;

	PlayerStateClass = ASPlayerState::StaticClass();  // Standard property of GameModeBase.h (UE parent class) - can also be set in editor (standard Player State Class) 
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	// Continuous timer to spawn in more bots.
	// Actual amount of bots and whether it's allowed to spawn determined by spawn logic later in the chain...
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
	

	// Assignment 5
	// Make sure we have assigned at least one power-up class
	if (ensure(PowerupClasses.Num() > 0))
	{
		// Skip the Blueprint wrapper and use the direct C++ option which the Wrapper uses as well
		FEnvQueryRequest Request(PowerupSpawnQuery, this);
		Request.Execute(EEnvQueryRunMode::AllMatching, this, &ASGameModeBase::OnPowerupSpawnQueryCompleted);
	}
}

void ASGameModeBase::OnPowerupSpawnQueryCompleted(TSharedPtr<FEnvQueryResult> Result)
{
	FEnvQueryResult* QueryResult = Result.Get();
	if (!QueryResult->IsSuccessful())
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}

	// Retrieve all possible locations that passed the query
	TArray<FVector> Locations;
	QueryResult->GetAllAsLocations(Locations);

	// Keep used locations to easily check distance between points
	TArray<FVector> UsedLocations;

	int32 SpawnCounter = 0;
	// Break out if we reached the desired count or if we have no more potential positions remaining
	while (SpawnCounter < DesiredPowerupCount && Locations.Num() > 0)
	{
		// Pick a random location from remaining points.
		int32 RandomLocationIndex = FMath::RandRange(0, Locations.Num() - 1);

		FVector PickedLocation = Locations[RandomLocationIndex];
		// Remove to avoid picking again
		Locations.RemoveAt(RandomLocationIndex);

		// Check minimum distance requirement
		bool bValidLocation = true;
		for (FVector OtherLocation : UsedLocations)
		{
			float DistanceTo = (PickedLocation - OtherLocation).Size();

			if (DistanceTo < RequiredPowerupDistance)
			{
				// Show skipped locations due to distance
				//DrawDebugSphere(GetWorld(), PickedLocation, 50.0f, 20, FColor::Red, false, 10.0f);

				// too close, skip to next attempt
				bValidLocation = false;
				break;
			}
		}

		// Failed the distance test
		if (!bValidLocation)
		{
			continue;
		}

		// Pick a random powerup-class
		int32 RandomClassIndex = FMath::RandRange(0, PowerupClasses.Num() - 1);
		TSubclassOf<AActor> RandomPowerupClass = PowerupClasses[RandomClassIndex];

		FVector spawnOffset = { 0, 0, 50 };
		GetWorld()->SpawnActor<AActor>(RandomPowerupClass, PickedLocation + spawnOffset, FRotator::ZeroRotator);

		// Keep for distance checks
		UsedLocations.Add(PickedLocation);
		SpawnCounter++;
	}
}

// MDJ: Lecture 14.4 add KillAll console command (Exec function)
void ASGameModeBase::KillAll()
{
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;


		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot); // MDJ: This is like our own version of a GameplayStatics function
		if (AttributeComp && AttributeComp->IsAlive())
		{
			AttributeComp->Kill(this); // @TODO: pass in player? for kill credit
		}
	}
}

void ASGameModeBase::SpawnBotTimerElapsed()
{	// MDJ: Lecture 15.3, early return if Console Variable CVarSpawnBots is false
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning disabled via cvar 'CVarSpawnBots'."));
		return;
	}

	// MDJ: Limit the number of AI spawned in the level at once using 'TActorIterator' -- "is like a better version of 'GetAllActorsOfClass'
	int32 NrOfAliveBots = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;


		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot); // MDJ: This is like our own version of a GameplayStatics function
		if (AttributeComp && AttributeComp->IsAlive())
		{
			NrOfAliveBots++;
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Found %i alive bots."), NrOfAliveBots);

	float MaxBotCount = 10.0f;
	// MDJ: Using a UCurveFloat to progressively increase difficulty -- assigned asset made in UE editor (right click > miscellaneous > curve)
	if (DifficultyFloat)
	{
		MaxBotCount = DifficultyFloat->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (NrOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("At maximum capacity. Skipping bot spawn."));
		return; // MDJ: "Early out" when not wanting to run the EQS query
	}
	// end of limiting spawn count




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

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	// MDJ: Check if returned locations array in fact has elements
	if (Locations.Num() > 0) // MDJ: Could also use Locations.IsValidIndex(0)
	{
		// MDJ: Spawn minion class at first array element
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);

		// Track all used spawn locations
		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false, 60.0f);
	}
}

void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		// MDJ: Remove PlayerCharacter from controller, so that when you call 'RestartPlayer' it does not simply teleport the existing PlayerCharacter
		//		CTRL+click 'RestartPlayer' to see in source code what it does if (NewPlayer->GetPawn() != nullptr)
		Controller->UnPossess();

		// MDJ: RestartPlayer is standard available in GameMode 
		RestartPlayer(Controller);
	}
}


void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	// Log Actor Killed to output log
	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));


	// Handle Player death
	ASCharacter* Player = Cast<ASCharacter>(VictimActor);
	if (Player)
	{
		// MDJ: TimerHandle purposely made local, so that if two players die in multiplayer they do not overwrite the same timer
		//		This however also means that we cannot cancel it anywhere, since we do not have the handle saved
		FTimerHandle TimerHandle_RespawnDelay;

		// MDJ: Little more difficult Timer than before, because we want to pass arguments to the Timer (i.e., who died, and thus who to respawn)
		//		Done using TimerDelegate
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());
		
		float RespawnDelay = 2.0f; // @TODO: this should be exposed as variable
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}


	// Give Credits for kill
	APawn* KillerPawn = Cast<APawn>(Killer);
	// Don't credit kills of self
	if (KillerPawn && KillerPawn != VictimActor)
	{
		// Only Players will have a 'PlayerState' instance, bots have nullptr
		ASPlayerState* PS = KillerPawn->GetPlayerState<ASPlayerState>();
		if (PS)
		{
			PS->AddCredits(CreditsPerKill);
		}
	}

}
