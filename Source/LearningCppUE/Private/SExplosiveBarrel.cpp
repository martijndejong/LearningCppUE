// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"

#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

#include "DrawDebugHelpers.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
	// MDJ: Create static mesh component and set it as root
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("BarrelStaticMesh");
	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->SetCollisionProfileName("PhysicsActor");
	RootComponent = StaticMesh;


	// MDJ: Add radial force component and attach to root
	RadialForce = CreateDefaultSubobject<URadialForceComponent>("RadialForceBarrel");
	RadialForce->SetupAttachment(StaticMesh);
	// MDJ: Set default values for radial force component
	RadialForce->Radius = 600.0f;
	RadialForce->ImpulseStrength = 2500.0f;
	RadialForce->bImpulseVelChange = true; // MDJ: ignores mass of object

	// RadialForce->AddCollisionChannelToAffect(ECC_WorldDynamic); // MDJ: not necessary because already in default radial force - but collision channels can be changed like this
}

// This is the initialization method for Unreal -- this runs between constructor and begin play
void ASExplosiveBarrel::PostInitializeComponents()
{
	// MDJ: Call parent initialization first, otherwise regular Actor init ignored
	Super::PostInitializeComponents();

	// MDJ: Bind OnComponentHit Event
	StaticMesh->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::ExplosionFunction); // MDJ: could have been put in BeginPlay or Constructor too, but if we use hot reloading it might not work
}


void ASExplosiveBarrel::ExplosionFunction(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForce->FireImpulse();

	// LOGGING -- shows up in UE Output Log and in VS: Unreal Engine Log during debug
	// Line by line debugging done by adding breakpoint during debug session and then F10 (into) F11 (over) to execute line by line -- F5 to continue
	// in BP a breakpoint can also be added by rightclicking on node > Add Breakpoint

	// Logging and Printing in C++ general tips:
	// https://nerivec.github.io/old-ue4-wiki/pages/logs-printing-messages-to-yourself-during-runtime.html

	// Logging example to make sure we reached the event
	// LogTemp = default category (there's also LogAI, LogRendering)
	// verbosity: Log, Warning, Error
	// use TEXT to convert string literal to Text type that UE_LOG needs
	UE_LOG(LogTemp, Log, TEXT("OnActorHit reached (ExplosiveBarrel)"));
	// Warning example:
	// MDJ: GetNameSafe() is same as GetName(), but returns empty string if Actor Ptr is NULL - instead of break. 
	// GetNameSafe needs asterix * in front to convert type from fstring to expected character array type -- not for float because we're directly printing as type float
	UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s, at game time: %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);

	// Draw string in 3d:
	// this one needed specific include! unlike UE_LOG
	// FString:Printf let's you format a string
	FString CombinedString = FString::Printf(TEXT("Hit at loctation: %s"), *Hit.ImpactPoint.ToString());
	DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true);
}

