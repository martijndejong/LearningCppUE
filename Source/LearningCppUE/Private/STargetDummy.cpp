// Fill out your copyright notice in the Description page of Project Settings.


#include "STargetDummy.h"
#include "Components/StaticMeshComponent.h"

#include "SAttributeComponent.h"

// Sets default values
ASTargetDummy::ASTargetDummy()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	// MDJ: Bind function to our own delegated Event macro (made in "SAttributeComponent.h":
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASTargetDummy::OnHealthChangedFunc);
}

void ASTargetDummy::OnHealthChangedFunc(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f) 
	{
		// MDJ: this below line is the equivalent of what is done in 'SineWaveBlueprint' BP through editor
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
}
