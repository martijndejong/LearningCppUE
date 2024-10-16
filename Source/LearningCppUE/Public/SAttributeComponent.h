// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

// MDJ: this is for making custom event macro (like the OnComponentBeginOverlap macro we used in MagicProjectile.cpp, but our own)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, USAttributeComponent*, OwningComp, float, NewHealth, float, Delta);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LEARNINGCPPUE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFUNCTION(BlueprintCallable, Category = "Attributes") // MDJ: Handy, BlueprintCallable makes it available as a node in BP
	// MDJ: Important to make it 'static' -- makes it not called on the component instance but on a static context
	static USAttributeComponent* GetAttributes(AActor* FromActor);
	
	UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (DisplayName = "IsAlive")) // MDJ: 'meta' gives options about how functions show up in editor
	static bool IsActorAlive(AActor* Actor); // MDJ: because it is marked as static, we need to pass Actor to the function now (it cannot grab it directly from the component instance)

	// Sets default values for this component's properties
	USAttributeComponent();

protected:
	// ------------------------------------------------------------------------------------------
	//								UPROPERTY SETTING DESCRIPTIONS
	// EditAnywhere - edit in BP editor and per-instance in level.
	// VisibleAnywhere - 'read-only' in editor and level. (Use for Components)
	// EditDefaultsOnly - hide variable per-instance, edit in BP editor only
	// VisibleDefaultsOnly - 'read-only' access for variable, only in BP editor (uncommon)
	// EditInstanceOnly - allow only editing of instance (eg. when placed in level)
	// --
	// BlueprintReadOnly - read-only in the Blueprint scripting (does not affect 'details'-panel)
	// BlueprintReadWrite - read-write access in Blueprints
	// --
	// Category = "" - display only for detail panels and blueprint context menu.
	// ------------------------------------------------------------------------------------------

	// MDJ: We put Attributes (e.g. Health) under 'Protected' because we do not want other classes to have direct access to it, we'll create a 'Public' function to make changes to this variable
	// MDJ: EditDefaultOnly (instead of EditAnywhere) we cannot change it on Instance of player in the level (click on object), but only in Class Default section -- to prevent confusing overexposure 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attributes")
	float HealthMax;
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	float Health;

public:	
	// MDJ: Here we put the function to change the Health, this will be 'Public' because we do want the outside to have access to it
	// MDJ: Make it a UFUNCTION(BlueprintCallable) so that it is exposed to BluePrints
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);


	// MDJ: Add declared event delegate -- if you now open PlayerCharacterBP in editor (which has AttributeComponent attached)
	//									   and you click on Attribute Comp, you'll see "On Health Changed" under the 'Events' tab
	// MDJ: FOnHealthChanged event delegate is declared at the top of this script using DECLARE_DYNAMIC_MULTICAST_DELEGATE
	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnHealthChanged OnHealthChanged;


	// MDJ: Public function because we want to access it from the animation graph
	// MDJ: and blueprint to access it from animBP
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsAlive() const; 
	// MDJ: adding 'const' (also in .cpp) makes it a const function -> a 'getter' function, it has read-only access to this information
	// that also means that the Blueprint function will no longer need an execution line (goes from Blue function with exec pin to Green function)
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsFullHealth() const;
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealth() const;
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealthMax() const;

	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatorActor);
						
};
