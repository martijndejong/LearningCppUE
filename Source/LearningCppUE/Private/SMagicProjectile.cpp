// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "SAttributeComponent.h"


// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// MDJ: Constructor: "make sure all of the components have an instance"

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
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
	

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->ProjectileGravityScale = 0.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
}

// MDJ: Apply damage to actor (if it has AttributeComponent)
void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator()) 
	{
		// MDJ: GetComponentByClass returns a UActorComponent*, thus a Cast must be added to convert to our AttributeComponent class
		// MDJ: ::StaticClass() can be added to get a reference to the static class (purple in BP)
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp)
		{
			// MDJ: Call our own ApplyHealthChange function of the custom AttributeComponent
			AttributeComp->ApplyHealthChange(-20.0f);

			Destroy();
		}
	}
}

// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();

	// MDJ: prevent self collision, learned this in BP in tutorial, now also done in C++
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
}

// Called every frame
void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

