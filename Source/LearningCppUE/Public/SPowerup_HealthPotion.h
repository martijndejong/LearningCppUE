// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerupActor.h"
#include "SPowerup_HealthPotion.generated.h"


class UStaticMeshComponent;


/**
 *
 */
UCLASS()
class LEARNINGCPPUE_API ASPowerup_HealthPotion : public ASPowerupActor
{
	GENERATED_BODY()

protected:

public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};