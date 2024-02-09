// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGameplayInterface.h"

// MDJ: for debugging only:
#include "DrawDebugHelpers.h"



// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void USInteractionComponent::PrimaryInteract()
{

	// MDJ: Look at what function expects, and then create all necesarry variables
	// FHitResult Hit; =============== COMMENTED INITIAL SOLUTION USING LINE TRACE
	FCollisionObjectQueryParams ObjectQueryParams;

	// MDJ: Add Object type to query collision for
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic); // to see what is part of query params, put a dot behind the instance and see options

	// Find Start and End
	AActor* MyOwner = GetOwner(); // from component this returns who you are attached to, so in this case our character
	FVector EyeLocation;
	FRotator EyeRotation;
	// MDJ: I did not like using GetActorEyesViewPoint, and wanted to use camera view point instead. Custom code for that is below
	if (APawn* MyPawn = Cast<APawn>(MyOwner)) {
		AController* MyController = MyPawn->GetController();
		MyController->GetPlayerViewPoint(EyeLocation, EyeRotation);
	}
	else {
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	}
	// end of custom code

	FVector Start = EyeLocation;
	FVector End = EyeLocation + EyeRotation.Vector()*1000;


	// MDJ: Just like Spawn Actor, collision queries are done through GetWorld
	// MDJ: The LineTraceSingleObjectType assigns values to the input variables it was given (so it puts a value in Hit, for example) -- the direct return is a bool if it hit anything
	// bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, Start, End, ObjectQueryParams); =============== COMMENTED INITIAL SOLUTION USING LINE TRACE

	// MDJ: Another version of this, SphereTrace instead of LineTrace:
	TArray<FHitResult> Hits; // returns multiple hits, so array instead
	FCollisionShape Shape; // needs to provide shape, we will use sphere
	float Radius = 30.0f;
	Shape.SetSphere(Radius); // set shape to sphere with radius ...f

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, Start, End, FQuat::Identity, ObjectQueryParams, Shape); // 'FQuat::Identity' here means no rotation applied, does not matter for our Sphere
	
	// MDJ: Set Color for Debugging Sphere
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red; // This is an inline if else statement

	// MDJ: Loop over the hit results and for each hit do the interaction (I made function InteractWithActor instead of having all the logic in the for loop itself)
	for (FHitResult Hit : Hits)
	{
		InteractWithActor(Hit, MyOwner);

		// MDJ: debugging draw sphere sweep:
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 2.0f);

		break; // MDJ: break out of for loop after interaction with first actor, so that one key press does not interact with multiple objects
	}

	DrawDebugLine(GetWorld(), Start, End, LineColor, false, 2.0f, 0, 2.0f);
}

// MDJ: InteractWithActor is something I added myself to not have to copy paste logic into for loop when going from line trace to sphere (sweep) trace
void USInteractionComponent::InteractWithActor(FHitResult Hit, AActor* MyOwner)
{
	AActor* HitActor = Hit.GetActor();
	// MDJ: Check if HitActor is not null, because we do not want to call function on null pointers
	if (HitActor)
	{
		// MDJ: Check if found Actor has USGameplayInterface (our interface class) implemented using "->Implements<Name>"
		if (HitActor->Implements<USGameplayInterface>())
		{
			// MDJ: Cast MyOwner to APawn type so that it can be passed to our interface interact function (requires Pawn instead of Actor)
			APawn* MyPawn = Cast<APawn>(MyOwner);

			// MDJ: very particular way of executing function of interface:
			//	reference interface and via 'Execute_<func_name>' execute the function
			//	however, even then the first parameter is not the input we specified (APawn* IstigatorPawn), but the actor we want to call the function on
			ISGameplayInterface::Execute_Interact(HitActor, MyPawn);
		}
	}
}
