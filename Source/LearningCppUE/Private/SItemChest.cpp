// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"

#include "Components/StaticMeshComponent.h"
// L19 include for DOREPLIFETIME:
#include "Net/UnrealNetwork.h"


// Sets default values
ASItemChest::ASItemChest()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110;

	// First thing to do when we want an actor to replicate between server and all clients is set replicate to true
	SetReplicates(true);
}


// MDJ: This is what happens when interface function is called
void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	// Tik tok between true and false
	bLidOpened = !bLidOpened;

	// Below code can be copied over to OnRep_LidOpened, 
	// because OnRep_LidOpened will called automatically ON CLIENT whenever the server updates bLidOpened
	// Because of our RepNotify (ReplicatedUsing="OnRep_LidOpened" in the SItemChest.h)
	// ----- moved ------
	//float CurrPitch = bLidOpened ? TargetPitch : 0.0f;
	//// Non-fancy C++ implementation of just directly setting the rotation (not animated like the BP solution we have)
	//LidMesh->SetRelativeRotation(FRotator(CurrPitch, 0, 0));
	// ----- moved ------
	OnRep_LidOpened(); // MANUAL CALL TO ALSO UPDATE ON SERVER, OTHERWISE YOU WILL ONLY SEE THE LID OPEN ON CLIENT
}

void ASItemChest::OnRep_LidOpened()
{
	float CurrPitch = bLidOpened ? TargetPitch : 0.0f;
	// Non-fancy C++ implementation of just directly setting the rotation (not animated like the BP solution we have)
	LidMesh->SetRelativeRotation(FRotator(CurrPitch, 0, 0));
}


// L19: Set up the rules for the replicated properties -- copy paste the below line whenever you use this, difficult to remember syntax for this
void ASItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Add properties to replicated for our ItemChest class
	// DOREPLIFETIME simply replicates it across all clients, 
	// there is also DOREPLIFETIME_CONDITION, where you could e.g., only Owner (COND_OwnerOnly) replicates - for optimization
	DOREPLIFETIME(ASItemChest, bLidOpened);
}