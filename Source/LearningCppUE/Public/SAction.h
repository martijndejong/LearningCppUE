// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SAction.generated.h"

class UWorld;

/**
 * 
 */
UCLASS(Blueprintable)  /* Have to mark this as 'Blueprintable' to make it able to have child class BP implementations (show up in 'New BP Class' wizzard whenever we make new BP class)
						(this is the first time we had to do this, because we derived from UObject, instead of UActor) */
class LEARNINGCPPUE_API USAction : public UObject
{
	GENERATED_BODY()

public: 

	// BlueprintNativeEvent: "has native implementation in C++, but Blueprint is allowed to overwrite that behavior and extend it"
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);


	/* Action nickname to start/stop without a reference to the object */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;  /* FName is hashed and therefore good for fast comparisons - FString would be checked per character, much slower
						FText only used for front facing text, FString only used for debugging and string manipulation) */


	// We need to override because we derived from UObject instead of AActor, meaning GetWorld is not automatically available
	UWorld* GetWorld() const override;
	
};
