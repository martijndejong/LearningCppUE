// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
// include SpringArmComponent and CameraComponent, find location by CTRL+click the <U...Component> after "CreateDefaultSubobject"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// MDJ: Create instance of SpringArmComponent and assign to pointer
	// MDJ: Make SpringArmComp child of RootComponent (like attaching it to the root in editor)
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// MDJ: Create MoveForward function to be passed to BindAxis
void ASCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// MDJ: Add BindAxis to PlayerInputComponent -- (1 axis name, 2 which character (self), 3 function to execute)
	// MDJ: name must be same as name added in Editor Input mappings
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);

	// MDJ: Same as above, but now for turn -- this time use &APawn because that already has function "AddControllerPitchInput"
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerPitchInput);
}

