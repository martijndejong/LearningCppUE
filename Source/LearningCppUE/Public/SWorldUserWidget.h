// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SWorldUserWidget.generated.h"


class USizeBox;

/**
 * 
 */
UCLASS()
class LEARNINGCPPUE_API USWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	// MDJ: 'Magic' trick to automatically fill pointer in editor so long as we have a matching variable name using 'meta = (BindWidget)'
	//		This basically forced the following compilation error when creating a BP instance of this class:
	//		"A required widget binding "ParentSizeBox" of type 'Size Box' was not found."
	UPROPERTY(meta = (BindWidget))
	USizeBox* ParentSizeBox;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override; // MDJ: this is the 'tick' equivalent of widgets

public: 

	UPROPERTY(EditAnywhere, Category = "UI")
	FVector WorldOffset;


	// MDJ: Made this public so that we can directly set it after we have created the widget (could also have made a protected function 'SetAttachedActor()' oid

	// MDJ: UPROPERTY for two reasons: 
	//		1) to be available in BP graph
	//		2) MORE IMPORTANT: broadcast to UE property system - good for reference counting -> now if AttachedActor get's destroyed, it will automatically clear the pointer
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	AActor* AttachedActor;
};
