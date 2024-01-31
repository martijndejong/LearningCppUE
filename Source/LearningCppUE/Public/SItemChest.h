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
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* LidMesh;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
