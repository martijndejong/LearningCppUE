// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

class ASPlayerState; // Forward declared to satisfy the delegate macros below


// Choice for using an event is going to help us chain things together. For example, our UI will be much better off using an event-based update rather than binding directly
//		UE Widgets offer 'binding' of element properties to certain variables, but those will then be updated every single frame - so more resource intensive, and e.g. cannot play animation on event
// Event Handler for Credits
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, ASPlayerState*, PlayerState, int32, NewCredits, int32, Delta);


/**
 *
 */
UCLASS()
class LEARNINGCPPUE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:

	// Put in protected so that we cannot accidentally access it from the outside - force using add/remove credit functions
	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	int32 Credits;


public:

	UFUNCTION(BlueprintCallable, Category = "Credits")
	int32 GetCredits() const;

	UFUNCTION(BlueprintCallable, Category = "Credits")
	void AddCredits(int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "Credits")
	bool RemoveCredits(int32 Delta);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCreditsChanged OnCreditsChanged;

};