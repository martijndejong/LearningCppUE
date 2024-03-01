// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// MDJ: imported for FMath::Clamp
#include "Math/UnrealMathUtility.h"


// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	HealthMax = 100.0f;
	Health = HealthMax;
}

bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
}

bool USAttributeComponent::IsAlive() const // MDJ: adding 'const' (also in .h) makes it a const function -> a 'getter' function, it has read-only access to this information
{
	return Health > 0.0f;
}

bool USAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

float USAttributeComponent::GetHealth() const
{
	return Health;
}

float USAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}


bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta) // MDJ: Make sure you do not call just 'Instigator' because that is already used in UE base class
{
	// MDJ: Lecture 14.4 -- checking 'CanBeDamaged' (standard in every actor) already implemented in 'CheatManager' under "God mode"
	//		Early out if actor cannot be damaged
	if (!GetOwner()->CanBeDamaged())
	{
		return false;
	}


	// MDJ: set old health to calculate ActualDelta and pass that to broadcast and return
	float OldHealth = Health;

	Health += Delta;
	Health = FMath::Clamp(Health, 0.0f, HealthMax); // MDJ: This can also be done in one line, usign "Health = FMath::Clamp(Health + Delta,  ...) "

	float ActualDelta = Health - OldHealth;
	// MDJ: Broadcast variables to 'OnHealthChanged' Event Delegate
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	return ActualDelta != 0;
}


// MDJ: STATIC functions:
USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	// MDJ: We even have to call GetAttributes here because we do not have direct access to the component instance (because STATIC)
	USAttributeComponent* AttributeComp = GetAttributes(Actor); // Not "USAttributeComponent::GetAttributes" because already in 'USAttributeComponent' namespace
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}
	return false;
}
