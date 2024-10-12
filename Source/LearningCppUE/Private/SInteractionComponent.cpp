// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGameplayInterface.h"

// MDJ: for debugging only:
#include "DrawDebugHelpers.h"


// Lecture 15.3: Console Variables -- more detailed comment in SGameModeBase.cpp
static TAutoConsoleVariable<bool>CVarDebugDrawInteraction(TEXT("su.InteractionDebugDraw"), false, TEXT("Enable Debug Lines for Interact Component."), ECVF_Cheat);



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

	// Lecture 19: Make it so that not every client runs the finding interaction actor function for every instance of interaction component
	// i.e., make it so that you do not run the calculations of finding interaction actor for other characters, but only run it for yourself
	// to do that, check if the pawn - that you are going to find interaction actor for - is locally controlled
	// Do this, because it is wastefull for everyone to run the Line Trace Queries for all other players
	APawn* MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn->IsLocallyControlled()) {
		// Finding interactable actor function I skipped from Lecture 18 goes here
	}
}


void USInteractionComponent::PrimaryInteract()
{
	// All the code that is in ServerInteract_Implementation was originalle here
	// I copied all the code into ServerInteract_Implementation and made this function call ServerInteract
	// Because I skipped lecture 16 - 18, I missed the updated version of the code of PrimaryInteract
	// So now there is a discrepance between what I copied into ServerInteract, and what Tom copied in there

	// Also skipped passing the Actor pointer to the ServerInteract function (since I don't have it set up like Lecture 18)
	// Passing this as a input parameter also makes it such that the server does not accidentally selects the wrong actor
	// because it was lagging and runs the actor selection query too late. 
	// Instead the Client will tell the server what actor it was trying to interact with
	ServerInteract();
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


// Add '_Implementation for UE boiler plate to work
void USInteractionComponent::ServerInteract_Implementation()
{
	// MDJ: Lecture 15.3 - grab the Console Variable for setting debug once
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();


	// MDJ: Look at what function expects, and then create all necesarry variables
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
	FVector End = EyeLocation + EyeRotation.Vector() * 1000;


	// MDJ: Just like Spawn Actor, collision queries are done through GetWorld
	// MDJ: The LineTraceSingleObjectType assigns values to the input variables it was given (so it puts a value in Hit, for example) -- the direct return is a bool if it hit anything
	// FHitResult Hit; =============== COMMENTED INITIAL SOLUTION USING LINE TRACE
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

		if (bDebugDraw)
		{
			// MDJ: debugging draw sphere sweep:
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 2.0f);
		}

		break; // MDJ: break out of for loop after interaction with first actor, so that one key press does not interact with multiple objects
	}

	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(), Start, End, LineColor, false, 2.0f, 0, 2.0f);
	}
}
