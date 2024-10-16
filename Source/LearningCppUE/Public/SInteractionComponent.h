// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LEARNINGCPPUE_API USInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

// MDJ: Make public function -- make it public because it will be called from character into this component
public:
	void PrimaryInteract();

public:	
	// Sets default values for this component's properties
	USInteractionComponent();

protected:
	// Lecture 19.2 Networking the world interaction logic:
	// Reliable - Will always arrive, eventually. Request will be re-sent unless an acknowledgement was received --> basically like TCP
	// Unreliable - Not guaranteed, packet can get lost and won't retry --> basically like UDP
	UFUNCTION(Server, Reliable)
	// Add function 'ServerInteract' -> RPC function naming convention: 'Server<Name>'/'Cient<Name>'/'MultiCast<Name>'
	void ServerInteract();
	
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

// MDJ: this is something I added myself to not have to copy paste logic into for loop when going from line trace to sphere (sweep) trace
private: 
	void InteractWithActor(FHitResult Hit, AActor* MyOwner);
};
