// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
// include SpringArmComponent and CameraComponent, find location by CTRL+click the <U...Component> after "CreateDefaultSubobject"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"


// This include is added for DEBUG ARROWS
#include "DrawDebugHelpers.h"


// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// MDJ: Create instance of SpringArmComponent and assign to pointer
	// MDJ: Make SpringArmComp child of RootComponent (like attaching it to the root in editor)
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);
	// MDJ: I had to add this code (not in Tom's lecture) to change the default values of SpringArmComp settings
	// MDJ: NOTE! the settings selected in the Blueprint will override the C++ defaults
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->bInheritPitch = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ==================== DEBUG ARROWS =======================
// This is entirely optional, it draws two arrows to visualize rotations of the player
// -- Rotation Visualization -- //
	const float DrawScale = 100.0f;
	const float Thickness = 5.0f;

	FVector LineStart = GetActorLocation();
	// Offset to the right of pawn
	LineStart += GetActorRightVector() * 100.0f;
	// Set line end in direction of the actor's forward
	FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	// Draw Actor's Direction
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);

	FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
	// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);
	// ==================== DEBUG ARROWS =======================
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// MDJ: Add BindAxis to PlayerInputComponent -- (1 axis name, 2 which character (self), 3 function to execute)
	// MDJ: name must be same as name added in Editor Input mappings
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	// MDJ: Same as above, but now for turn -- this time use &APawn because that already has function "AddControllerYawInput"
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	// MDJ: same same but for pitch
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}


// MDJ: Create MoveForward function to be passed to BindAxis
void ASCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

// MDJ: Create MoveRight function to be passed to BindAxis
void ASCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

