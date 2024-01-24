// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"

#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
	// Create static mesh component and set it as root
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("BarrelStaticMesh");
	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->SetCollisionProfileName("PhysicsActor");
	RootComponent = StaticMesh;


	// Add radial force component and attach to root
	RadialForce = CreateDefaultSubobject<URadialForceComponent>("RadialForceBarrel");
	RadialForce->SetupAttachment(StaticMesh);
	// Set default values for radial force component
	RadialForce->Radius = 600.0f;
	RadialForce->ImpulseStrength = 2500.0f;
	RadialForce->bImpulseVelChange = true;
}

// This is the initialization method for Unreal
void ASExplosiveBarrel::PostInitializeComponents()
{
	// Call parent initialization first, otherwise regular Actor init ignored
	Super::PostInitializeComponents();

	// Bind OnComponentHit Event
	StaticMesh->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::ExplosionFunction);
}


void ASExplosiveBarrel::ExplosionFunction(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForce->FireImpulse();

	// Logging example to make sure we reached the event
	UE_LOG(LogTemp, Log, TEXT("OnActorHit reached (ExplosiveBarrel)"));
}

