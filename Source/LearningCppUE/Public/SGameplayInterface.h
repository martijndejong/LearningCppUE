// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SGameplayInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USGameplayInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LEARNINGCPPUE_API ISGameplayInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// MDJ: BlueprintNativeEvent: native here is C++, this is both for C++ and BP -> when added to C++ class, code expects implementation but can still be added to BP
	// MDJ: BlueprintImplementableEvent: is really used if intent to only have implementation in BP,  not in C++
	// MDJ: the above made it so that we could add the interface to the Lever and implement an event for the interface in the Event Graph
	// MDJ: by also adding 'BlueprintCallable', we make it so that we can call the Interact interface function from a BP
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact(APawn* InstigatorPawn);
};
