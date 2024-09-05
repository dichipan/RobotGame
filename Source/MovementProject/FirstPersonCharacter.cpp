#include "FirstPersonCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include <Net/UnrealNetwork.h>
#include "Engine/World.h"

// Sets default values
AFirstPersonCharacter::AFirstPersonCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	canLook = true;
	cameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Spring Arm"));
	cameraSpringArm->SetupAttachment(RootComponent);
	cameraSpringArm->TargetArmLength = 0.0f;
	cameraSpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	cameraSpringArm->bUsePawnControlRotation = true;

	weaponSlot = CreateDefaultSubobject<UChildActorComponent>(TEXT("Weapon Slot"));
	weaponSlot->SetupAttachment(cameraSpringArm);
	weaponSlot->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	playerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	playerCamera->SetupAttachment(cameraSpringArm, USpringArmComponent::SocketName);
	playerCamera->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void AFirstPersonCharacter::BeginPlay()
{
	Super::BeginPlay();
	FSlateApplication::Get().OnApplicationActivationStateChanged().AddUObject(this, &AFirstPersonCharacter::OnWindowFocusChanged);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}

	GetCharacterMovement()->AirControl = 0.1f;
	GetCharacterMovement()->MaxWalkSpeed = speed;
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->GroundFriction = 0.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1800.0f;
	jumpsLeft = numberOfJumps;
	inFocus = true;
	playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	GEngine->GameViewport->GetViewportSize(viewportSize);
	viewportSize.X /= 2;
	viewportSize.Y /= 2;
	playerCamera->FieldOfView = fieldOfView;
	currentSensitivity = sensitivity;
}

void AFirstPersonCharacter::AddPoints(int addedPoints)
{
	points += addedPoints;
}

// Called every frame
void AFirstPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	movingCamera = false;
	if (inFocus) {
		playerController->SetMouseLocation(viewportSize.X, viewportSize.Y);
	}
}

// Called to bind functionality to input
void AFirstPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &AFirstPersonCharacter::PlayerJump);
		EnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Started, this, &AFirstPersonCharacter::SprintPressed);
		EnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &AFirstPersonCharacter::SprintReleased);
	}
	PlayerInputComponent->BindAxis("LookHorizontal", this, &AFirstPersonCharacter::LookHorizontal);
	PlayerInputComponent->BindAxis("LookVertical", this, &AFirstPersonCharacter::LookVertical);
	PlayerInputComponent->BindAxis("MoveHorizontal", this, &AFirstPersonCharacter::MoveHorizontal);
	PlayerInputComponent->BindAxis("MoveVertical", this, &AFirstPersonCharacter::MoveVertical);
}

// When focus on window is lost, set inFocus false
void AFirstPersonCharacter::OnWindowsLostFocus()
{
	inFocus = false;
}

// When focus on window is gained, set inFocus true
void AFirstPersonCharacter::OnWindowsGainFocus()
{
	inFocus = true;
}

// Triggered when window focus is changed
void AFirstPersonCharacter::OnWindowFocusChanged(const bool bIsFocused)
{
	if (bIsFocused)
	{
		OnWindowsGainFocus();
	}
	else
	{
		OnWindowsLostFocus();
	}
}

// Controls horizontal A/D movement
void AFirstPersonCharacter::MoveHorizontal(float Axis)
{
	if (IsPlayerControlled()) {
		FRotator Rotation = Controller->GetControlRotation();
		FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
		FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightVector, Axis);
	}

	// Set dynamic camera rotation
	float newRot = FMath::FInterpTo(playerCamera->GetRelativeRotation().Roll, Axis * 0.5f, FApp::GetDeltaTime(), 10.0f);
	FRotator rotation = FRotator(playerCamera->GetRelativeRotation().Pitch, playerCamera->GetRelativeRotation().Yaw, newRot);
	FQuat newRotation = rotation.Quaternion();
	playerCamera->SetRelativeRotation(newRotation);

	if (!isAiming) {
		// Set dynamic weapon location, Y
		float newLoc = FMath::FInterpTo(weaponSlot->GetRelativeLocation().Y, Axis * 5.0f, FApp::GetDeltaTime(), 1.5f);
		FVector location = FVector(weaponSlot->GetRelativeLocation().X, newLoc, weaponSlot->GetRelativeLocation().Z);
		weaponSlot->SetRelativeLocation(location);

		// Set dynamic weapon rotation, roll + pitch
		float newRot2 = FMath::FInterpTo(weaponSlot->GetRelativeRotation().Roll, Axis * 10.0f, FApp::GetDeltaTime(), 2.5f);
		FRotator rotation2 = FRotator(weaponSlot->GetRelativeRotation().Pitch, weaponSlot->GetRelativeRotation().Yaw, newRot2);
		FQuat newRotation2 = rotation2.Quaternion();
		weaponSlot->SetRelativeRotation(newRotation2);
	}
}

// Controls vertical W/S movement
void AFirstPersonCharacter::MoveVertical(float Axis)
{
	if (IsPlayerControlled()) {
		FRotator Rotation = Controller->GetControlRotation();
		FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
		FVector LeftVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(LeftVector, Axis);
		moveVerticalAxis = Axis;
		//if (Axis <= 0 && isSprinting) SprintEnd();
	}

	// Set dynamic camera rotation
	float newRot = FMath::FInterpTo(playerCamera->GetRelativeRotation().Pitch, Axis * -0.5f, FApp::GetDeltaTime(), 10.0f);
	FRotator rotation = FRotator(newRot, playerCamera->GetRelativeRotation().Yaw, playerCamera->GetRelativeRotation().Roll);
	FQuat newRotation = rotation.Quaternion();
	playerCamera->SetRelativeRotation(newRotation);

	if (!isAiming) {
		// Set dynamic weapon location, X and Z
		float newLoc = FMath::FInterpTo(weaponSlot->GetRelativeLocation().X, FMath::Clamp(Axis * -2.0f, -2, 0), FApp::GetDeltaTime(), 1.5f);
		float newLoc2 = FMath::FInterpTo(weaponSlot->GetRelativeLocation().Z, FMath::Clamp(Axis * -7.0f, -7, 2), FApp::GetDeltaTime(), 1.5f);
		FVector location = FVector(newLoc, weaponSlot->GetRelativeLocation().Y, newLoc2);
		weaponSlot->SetRelativeLocation(location);
	}
}

// Controls horizontal mouse movement
void AFirstPersonCharacter::LookHorizontal(float Axis)
{
	if (canLook)
	{
		AddControllerYawInput(Axis * currentSensitivity);
		if (Axis != 0) {
			movingCamera = true;
			lookRotation = GetControlRotation();
			StopWeaponRecoil(); // BANDAGE FIX
		}
	}
	if (!isAiming) {
		// Set dynamic weapon location
		float newLoc = FMath::FInterpTo(weaponSlot->GetRelativeLocation().Y, FMath::Clamp(Axis, -1.5f, 1.5f), FApp::GetDeltaTime(), 2.5f);
		FVector location = FVector(weaponSlot->GetRelativeLocation().X, newLoc, weaponSlot->GetRelativeLocation().Z);
		weaponSlot->SetRelativeLocation(location);

		// Set dynamic weapon rotation
		float newRot = FMath::FInterpTo(weaponSlot->GetRelativeRotation().Yaw, FMath::Clamp(Axis * -2.0f, -5.0f, 5.0f), FApp::GetDeltaTime(), 3.0f);
		float newRot2 = FMath::FInterpTo(weaponSlot->GetRelativeRotation().Roll, FMath::Clamp(Axis * -2.0f, -5.0f, 5.0f), FApp::GetDeltaTime(), 3.0f);
		FRotator rotation = FRotator(weaponSlot->GetRelativeRotation().Pitch, newRot, newRot2);
		FQuat newRotation = rotation.Quaternion();
		weaponSlot->SetRelativeRotation(newRotation);
	}
	else {
		// Set dynamic weapon location
		float newLoc = FMath::FInterpTo(weaponSlot->GetRelativeLocation().Y, FMath::Clamp(Axis * 0.5f, -0.25f, 0.25f), FApp::GetDeltaTime(), 5.0f);
		FVector location = FVector(weaponSlot->GetRelativeLocation().X, newLoc, weaponSlot->GetRelativeLocation().Z);
		weaponSlot->SetRelativeLocation(location);
	}
}

// Controls vertical mouse movement
void AFirstPersonCharacter::LookVertical(float Axis)
{
	if (canLook)
	{
		AddControllerPitchInput(Axis * currentSensitivity);
		if (Axis != 0) {
			movingCamera = true;
			lookRotation = GetControlRotation();
			StopWeaponRecoil(); // BANDAGE FIX
		}
	}
	if (!isAiming) {
		// Set dynamic weapon location
		float newLoc = FMath::FInterpTo(weaponSlot->GetRelativeLocation().Z, FMath::Clamp(Axis * -1, -2.0f, 2.0f), FApp::GetDeltaTime(), 2.5f);
		FVector location = FVector(weaponSlot->GetRelativeLocation().X, weaponSlot->GetRelativeLocation().Y, newLoc);
		weaponSlot->SetRelativeLocation(location);

		// Set dynamic weapon rotation
		float newRot = FMath::FInterpTo(weaponSlot->GetRelativeRotation().Pitch, FMath::Clamp(Axis * 5.0f, -10.0f, 10.0f), FApp::GetDeltaTime(), 1.5f);
		FRotator rotation = FRotator(newRot, weaponSlot->GetRelativeRotation().Yaw, weaponSlot->GetRelativeRotation().Roll);
		FQuat newRotation = rotation.Quaternion();
		weaponSlot->SetRelativeRotation(newRotation);
	}
	else {
		// Set dynamic weapon location
		float newLoc = FMath::FInterpTo(weaponSlot->GetRelativeLocation().Z, FMath::Clamp(Axis * -0.5, -0.25f, 0.25f), FApp::GetDeltaTime(), 5.0f);
		FVector location = FVector(weaponSlot->GetRelativeLocation().X, weaponSlot->GetRelativeLocation().Y, newLoc);
		weaponSlot->SetRelativeLocation(location);
	}
}

// Triggered when player lands
void AFirstPersonCharacter::Landed(const FHitResult& Hit)
{
	playerController->PlayerCameraManager->StartCameraShake(landShake);
	isGrounded = true;
	jumpsLeft = numberOfJumps;
	SprintStart();
}

// Triggered when player walks off a ledge
void AFirstPersonCharacter::OnWalkingOffLedge_Implementation(const FVector& PreviousFloorImpactNormal, const FVector& PreviousFloorContactNormal, const FVector& PreviousLocation, float TimeDelta)
{
	SprintEnd();
	jumpsLeft--;
	isGrounded = false;
}

// Launches player in the air
void AFirstPersonCharacter::PlayerJump()
{
	if (jumpable && (isGrounded || jumpsLeft > 0)) {
		SprintEnd();
		if (isCrouching) CrouchReleased();
		if (jumpsLeft == numberOfJumps) {
			LaunchCharacter(FVector(0.0f, 0.0f, jumpHeight), false, true);
		}
		else {
			LaunchCharacter(FVector(0.0f, 0.0f, secondaryJumpHeight), false, true);
		}
		isGrounded = false;
		jumpsLeft--;
	}
}

// Handles client-sided sprint start
void AFirstPersonCharacter::SprintStart()
{
	if ((holdingSprint && isGrounded)) {
		if (!isCrouching) {
			GetCharacterMovement()->MaxWalkSpeed = sprintSpeed;
			SprintShakeStart();
			isSprinting = true;
		}
		else {
			if (moveVerticalAxis > 0) {
				CrouchReleased();
				SprintStart();
			}
		}
	}
}

// Handles client-sided sprint end
void AFirstPersonCharacter::SprintEnd()
{
	if (!isCrouching) {
		GetCharacterMovement()->MaxWalkSpeed = speed;
	}
	SprintShakeEnd();
	isSprinting = false;
}

// Activates when sprint is pressed
void AFirstPersonCharacter::SprintPressed()
{
	holdingSprint = true;
	SprintStart();
}

// Activates when sprint is released
void AFirstPersonCharacter::SprintReleased()
{
	holdingSprint = false;
	//if (holdToSprint) SprintEnd();
	SprintEnd();
}

