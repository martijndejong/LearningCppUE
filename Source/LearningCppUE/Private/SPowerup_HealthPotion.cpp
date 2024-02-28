// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerup_HealthPotion.h"
#include "SAttributeComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SPowerup_HealthPotion)


void ASPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth())
	{
		// MDJ: Call our own ApplyHealthChange function of the custom AttributeComponent
		// MDJ: Should make the 50.0f a tweakable variable
		AttributeComp->ApplyHealthChange(this, +50.0f);

		HideAndCooldownPowerup();
	}
}