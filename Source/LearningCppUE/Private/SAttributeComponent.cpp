// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	Health = 100;
}

bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);

	return true;
}

bool USAttributeComponent::IsAlive() const // MDJ: adding 'const' (also in .h) makes it a const function -> a 'getter' function, it has read-only access to this information
{
	return Health > 0.0f;
}
