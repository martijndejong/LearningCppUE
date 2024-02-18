// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
// MDJ: include SpringArmComponent and CameraComponent, find location by CTRL+click the <U...Component> after "CreateDefaultSubobject"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// MDJ: include our own component classes
#include "SInteractionComponent.h"
#include "SAttributeComponent.h"

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

	// MDJ: Adding the selfmade interaction component
	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	// MDJ: Adding selfmade attribute component
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");


	// MDJ: Fixing movement / lookaround input
	// MDJ: below set direct property of ASCharacter (inherited from Actor parent class) to enable looking around without yawing character
	bUseControllerRotationYaw = false;
	// needed import -> I ctrl+clicked on the fuction to find cpp file, then went to top to find corresponding header file
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// MDJ: binding function to AttributeComp 'OnHealthChanged' event -- this function checks if player died and disables input
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChangedFunc);
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

	// MDJ: assignment 2: add SecondaryAttack (blackhole) and dash
	PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &ASCharacter::BlackholeAttack);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ASCharacter::Dash);

	// MDJ: Assignment 1: add character jump
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	//MDJ: Bind action for interaction component
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
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
	// MDJ: Add animation to PrimaryAttack
	PlayAnimMontage(AttackAnim);

	// MDJ: Delay the animation -- should be done using Animation Notifies, but for illustrative purposes a Timer is explained and used here
	float AnimationDelay = 0.2f;
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, AnimationDelay);
	// GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack); <--- this would clear the timer, could be used e.g. when character dies so that it does not fire after death

	// MDJ: Previously the code of PrimaryAttack_TimeElapsed was here, but it has been moved into that function so that it can executed by the timer
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	SpawnProjectile(ProjectileClass);
}

void ASCharacter::BlackholeAttack()
{
	PlayAnimMontage(AttackAnim);
	float AnimationDelay = 0.2f;
	GetWorldTimerManager().SetTimer(TimerHandle_BlackholeAttack, this, &ASCharacter::BlackholeAttack_TimeElapsed, AnimationDelay);
}

void ASCharacter::BlackholeAttack_TimeElapsed()
{
	SpawnProjectile(BlackholeProjectileClass);
}

void ASCharacter::Dash()
{
	SpawnProjectile(DashProjectileClass);
}

// MDJ: Note: we create a new function PrimaryInteract here for character class that is bound to action
//		this is a completely separate function from the InteractionComponent 'PrimaryInteract', they just have the same name 
void ASCharacter::PrimaryInteract()
{
	InteractionComp->PrimaryInteract();
}


// MDJ: first the logic below was directly in PrimaryAttack, but it has been made re-usable below:
void ASCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	// MDJ: only run this code if ProjectileClass has been assigned (prevent NullPtr error)
	// MDJ: 'ensure' is UE macro that works as a breakpoint if ProjectileClass=NullPtr and simply extends the regular if() behavior if ProjectileClass is assigned
	// MDJ: it only highlights the breakpoint on first Play after compile, if it needs to always bring up the breakpoint use 'ensureAlways'
	// MDJ: Upon compilation, 'ensure' does not do anything, so below code would effectively just be "if (ProjectileClass)" 
	if (ensure(ClassToSpawn)) {
		// MDJ: Inputs for SpawnActor (last line of this function) explained:
		// MDJ: FIRST INPUT 
		// For the first input of SpawnActor we need to pass a class, to do this we need to add new UPROPERTY 'ProjectileClass' to SCharacter.h

		// MDJ: SECOND INPUT 
		// Set up Transformation Matrix to use for spawning actor -- first pass rotator (GetControlRotation) then vector (GetActorLocation -- or socket location)
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
		// FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation);
		// =================== ASSIGNMENT 2.1 // =================== 
		//				Fix Projectile Launch Direction
		// line trace
		FHitResult Hit;
		FVector Start;
		FVector End;
		FCollisionObjectQueryParams ObjectQueryParams;
		bool bBlockingHit;

		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Vehicle);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Destructible);

		FVector CameraLocation;
		FRotator CameraRotation;
		GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);

		Start = CameraLocation;
		End = Start + CameraRotation.Vector() * 99999;

		// MDJ: Tom uses a 'SweepSingleByObjectType' and passes a Sphere (radius 20.0f) as collision shape (thus making it a SphereTrace)  -> to make it easier to hit something
		bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, Start, End, ObjectQueryParams);

		// Compute Spawn Transform
		FRotator SpawnRotation = bBlockingHit ? (Hit.ImpactPoint - HandLocation).Rotation() : CameraRotation;
		FTransform SpawnTM = FTransform(SpawnRotation, HandLocation);

		FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red; // This is an inline if else statement
		DrawDebugLine(GetWorld(), Start, Hit.ImpactPoint, LineColor, false, 2.0f, 0, 2.0f);
		// =================== ASSIGNMENT 2.1 // =================== 

		// MDJ: THIRD INPUT
		// Set up spawn parameters to use for spawning (many options to change, but we only interested in SpawnCollisionHandlingOverride (set to always Spawn)
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		// MDJ: THIS IS IMPORTANT TO AVOID SELF COLLISION! Pass self as 'instigator' to SpawnActor
		SpawnParams.Instigator = this;

		// MDJ: We want to spawn the projectile actor. Spawning is always done through the world, so starts with GetWorld()
		// MDJ: SpawnActor first expects <type> , so <AActor>, and then (class, transform, parameters)
		GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);
	}
}


// MDJ: Prevent player from moving around when dead
void ASCharacter::OnHealthChangedFunc(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		// MDJ: hit flash effect -- same as 'TargetDummy'
		GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}

	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		DisableInput(PlayerController);
	}
}
