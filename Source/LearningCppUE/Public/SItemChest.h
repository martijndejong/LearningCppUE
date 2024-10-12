// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SGameplayInterface.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
// MDJ: To add interface, add it where we also derive from Actor
class LEARNINGCPPUE_API ASItemChest : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	float TargetPitch;
	
	// MDJ: because we chose 'BlueprintNativeEvent', the boilerplate of UE expect <func_name>_Implementation (otherwise would've just been <func_name>)
	void Interact_Implementation(APawn* InstigatorPawn);

public:	
	// Sets default values for this actor's properties
	ASItemChest();

protected:
	// L19: add new state that we want to be replicated between server and all clients
	// --> Only 'UPROPERTY(Replicated)' would replicate the value across server and clients, but 'ReplicatedUsing' calls a function too, 
	// in this case, the animating of actually opening and closing the chest on client too 
	UPROPERTY(ReplicatedUsing="OnRep_LidOpened") // "RepNotify" 
	bool bLidOpened;

	// Function that will be called by our RepNotify
	UFUNCTION()
	void OnRep_LidOpened();

	// This below declaration seems to be redundant, as it works without, but this was included in the header in unreal documentation example
	// Override Replicate Properties function
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BaseMesh;

	// MDJ: Had to add BlueprintReadOnly so that it could be used in Event Graph 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;

};
