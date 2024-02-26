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

bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	// MDJ: set old health to calculate ActualDelta and pass that to broadcast and return
	float OldHealth = Health;

	Health += Delta;
	Health = FMath::Clamp(Health, 0.0f, HealthMax); // MDJ: This can also be done in one line, usign "Health = FMath::Clamp(Health + Delta,  ...) "

	float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(nullptr, this, Health, ActualDelta);

	return ActualDelta != 0;
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
