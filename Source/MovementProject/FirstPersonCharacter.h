#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraShakeBase.h"
#include "FirstPersonCharacter.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class MOVEMENTPROJECT_API AFirstPersonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFirstPersonCharacter();

protected:
	virtual void BeginPlay() override;

	// Player settings

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSettings)
	float fieldOfView = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSettings)
	float sensitivity = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSettings)
	float currentSensitivity = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSettings)
	bool holdToCrouch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSettings)
	bool holdToSprint;

	// Character settings

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats)
	float healthCurrent = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats)
	float healthMax = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats)
	int points = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats)
	float speed = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats)
	float sprintSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool crouchable = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float crouchSpeed = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool jumpable = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats)
	int numberOfJumps = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats)
	int jumpsLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats)
	float jumpHeight = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats)
	float secondaryJumpHeight = 500.0f;

	UPROPERTY(BlueprintReadWrite)
	bool movingCamera;

	UPROPERTY(BlueprintReadWrite)
	bool isGrounded = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool holdingAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isAiming = false;

	UPROPERTY(BlueprintReadWrite)
	bool isCrouching;
	UPROPERTY(BlueprintReadWrite)
	bool canUncrouch;

	UPROPERTY(BlueprintReadWrite)
	FRotator lookRotation;

	UPROPERTY(BlueprintReadWrite)
	bool isSprinting;
	UPROPERTY(BlueprintReadWrite)
	bool holdingSprint;

	UPROPERTY(BlueprintReadWrite)
	bool inFocus;

	UPROPERTY(BlueprintReadWrite)
	bool canLook;

	// Other

	APlayerController* playerController;
	FVector2D viewportSize;
	float moveVerticalAxis;

	/* FUNCTIONS */

	// Basic Movement
	void MoveHorizontal(float Axis);
	void MoveVertical(float Axis);
	void LookHorizontal(float Axis);
	void LookVertical(float Axis);
	void Landed(const FHitResult& Hit) override;
	void OnWalkingOffLedge_Implementation(const FVector& PreviousFloorImpactNormal, const FVector& PreviousFloorContactNormal, const FVector& PreviousLocation, float TimeDelta) override;
	UPROPERTY(EditAnywhere, Category = CameraShakes)
	TSubclassOf<UCameraShakeBase> landShake;

	// Advanced Movement
	UFUNCTION(BlueprintCallable)
	void PlayerJump();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void CrouchPressed();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void CrouchReleased();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool CanUncrouchCheck();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void StopWeaponRecoil();

	// Sprint
	UFUNCTION(BlueprintCallable)
	void SprintStart();
	void SprintPressed();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SprintShakeStart();

	UFUNCTION(BlueprintCallable)
	void SprintEnd();
	void SprintReleased();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SprintShakeEnd();

	void Interact();

	UFUNCTION(BlueprintCallable)
	void AddPoints(int addedPoints);

	// Inputs
	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IA_Jump;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IA_Sprint;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IA_LookVertical;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IA_LookHorizontal;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IA_MoveVertical;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* IA_MoveHorizontal;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Window Focus
	void OnWindowsLostFocus();
	void OnWindowsGainFocus();

private:
	// Window Focus Change
	void OnWindowFocusChanged(bool bIsFocused);

	// UComponents
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CameraSpringArm", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* cameraSpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* playerCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* weaponSlot;

};
