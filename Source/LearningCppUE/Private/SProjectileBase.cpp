// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileBase.h"

#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Components/AudioComponent.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ASProjectileBase::ASProjectileBase()
{
	// MDJ: Add SphereComponent as root 
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	RootComponent = SphereComp;
	// MDJ: Collision settings can be changed in BP editor, but also here by default
	// MDJ: Scalable way is kept uncommented: create new custom collision profile in editor and assign it to SphereComp
	SphereComp->SetCollisionProfileName("Projectile");
	// MDJ: Alternatively, you can also set all the collision properties here immediately, as shown in comments below
	//SphereComp->SetCollisionObjectType(ECC_WorldDynamic);
	//SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	//SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// MDJ: Adding functionality on component overlap -- this requires a function to be created that executes, we called it &ASMagicProjectile::OnActorOverlap
	// MDJ: that function needs specific, long list of inputs to fit the OnComponentBeginOverlap.AddDynamic
	// MDJ: Go to definition of OnComponentBeginOverlap, then go to definition of FComponentBeginOverlapSignature to find all those inputs that can then be used in function declaration
	// SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap); ------ this is left in original script


	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	LoopSoundComp = CreateDefaultSubobject<UAudioComponent>("LoopSoundComp");
	LoopSoundComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->ProjectileGravityScale = 0.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;

	ImpactShakeInnerRadius = 0.0f;
	ImpactShakeOuterRadius = 1500.0f;
}

void ASProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);

	SphereComp->OnComponentHit.AddDynamic(this, &ASProjectileBase::OnActorHit);
}

void ASProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	LoopSoundComp->Play();
}

void ASProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

// <event name>_Implementation is necesarry since 'Explode' event is marked as BlueprintNativeEvent in SProjectileBase.h
void ASProjectileBase::Explode_Implementation()
{
	// Check to make sure we aren't already being 'destroyed'
	// Adding 'ensure' to see if we encounter this situation at all
	if (ensure(IsValid(this)))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

		UGameplayStatics::PlaySoundAtLocation(this, ImpactSFX, GetActorLocation());

		LoopSoundComp->Stop();

		UGameplayStatics::PlayWorldCameraShake(this, ImpactCameraShake, GetActorLocation(), ImpactShakeInnerRadius, ImpactShakeOuterRadius);

		Destroy();
	}
}
