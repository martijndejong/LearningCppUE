// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
// MDJ: include SpringArmComponent and CameraComponent, find location by CTRL+click the <U...Component> after "CreateDefaultSubobject"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// MDJ: Include this to enable 'GetCharacterMovement' 
# include "GameFramework/CharacterMovementComponent.h"


// MDJ: This include is added for DEBUG ARROWS
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

	// MDJ: below set direct property of ASCharacter (inherited from Actor parent class) to enable looking around without yawing character
	bUseControllerRotationYaw = false;

	// needed import -> I ctrl+clicked on the fuction to find cpp file, then went to top to find corresponding header file
	GetCharacterMovement()->bOrientRotationToMovement = true;
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


	// MDJ: Bind action for spawning MagicProjectile
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);

	// MDJ: Assignment 1: add character jump
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
}


// MDJ: Create MoveForward function to be passed to BindAxis
void ASCharacter::MoveForward(float Value)
{
	// MDJ: The below code makes it so that the character moves in the direction that the camera is facing (control rotation ipv character rotation)
	// MDJ: First make new FRotator named ControlRot and give it the output of GetControlRotation, 
	// then force pitch & roll =0, then take vector value and pass to AddMovementInput
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	AddMovementInput(ControlRot.Vector(), Value);

	// MDJ: this was the initial simple move forward:
	// AddMovementInput(GetActorForwardVector(), Value);
}

// MDJ: Create MoveRight function to be passed to BindAxis
void ASCharacter::MoveRight(float Value)
{
	// MDJ: The below code makes it so that the character moves right and left compared to where camera is facing (control rotation ipv character rotation)
	// MDJ: First make new FRotator named ControlRot and give it the output of GetControlRotation, 
	// then force pitch & roll =0, then take vector value and pass to AddMovementInput
	// then take the orthogonal direction:: two options: 
	//	Vector().RotateAngleAxis(90, {0, 0, 1}) 
	//	or look up source code for GetRightVector (KismetLibrary) and use their function (FRotationMatrix)
	// MDJ: PRO TIP: to find the GetRightVector and usage -- you can use BluePrint to search it 
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);

	// MDJ: this was the initial simple move right:
	// AddMovementInput(GetActorRightVector(), Value);
}

// MDJ: This is the function that will be executed by BindAction that is referencing this function
void ASCharacter::PrimaryAttack()
{
	// MDJ: FIRST INPUT 
	// For the first input of SpawnActor we need to pass a class, to do this we need to add new UPROPERTY 'ProjectileClass' to SCharacter.h
	
	// MDJ: SECOND INPUT 
	// Set up Transformation Matrix to use for spawning actor -- first pass rotator (GetControlRotation) then vector (GetActorLocation -- or socket location)
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation);

	// MDJ: THIRD INPUT
	// Set up spawn parameters to use for spawning (many options to change, but we only interested in SpawnCollisionHandlingOverride (set to always Spawn)
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	// MDJ: We want to spawn the projectile actor. Spawning is always done through the world, so starts with GetWorld()
	// MDJ: SpawnActor first expects <type> , so <AActor>, and then (class, transform, parameters)
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
}

