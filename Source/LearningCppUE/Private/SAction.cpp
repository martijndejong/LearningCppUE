// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"



void USAction::StartAction_Implementation(AActor* Instigator)  // Have the add '_Implementation' because the function is marked as BlueprintNativeEvent
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
}


void USAction::StopAction_Implementation(AActor* Instigator)  // Have the add '_Implementation' because the function is marked as BlueprintNativeEvent
{
	UE_LOG(LogTemp, Log, TEXT("Stopping: %s"), *GetNameSafe(this));
}

// Overriding this was necessary because we are dealing with UObjects
UWorld* USAction::GetWorld() const
{
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());  // 'GetOuter()' returns our ActionComponent because we set that in SActionComponent.cpp when we add the action as a new object
	if (Comp)  // This check is necessary because within the editor our UObject might call GetWorld, which at that point will not have our Outer set to our ActionComponent, like we have
	{
		return Comp->GetWorld();
	}

	return nullptr;
}
