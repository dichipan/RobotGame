#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "FirstPersonCharacter.generated.h"

UCLASS()
class MOVEMENTPROJECT_API AFirstPersonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFirstPersonCharacter();

protected:
	virtual void BeginPlay() override;

	// Player settings

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float fieldOfView = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float sensitivity = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool holdToCrouch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool holdToSprint;

	// Character settings

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float healthCurrent = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float healthMax = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float armorCurrent = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float armorMax = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int points = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float speed = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float sprintSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool crouchable = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float crouchSpeed = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool jumpable = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int numberOfJumps = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int jumpsLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float jumpHeight = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
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
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void LandedShake();
	void OnWalkingOffLedge_Implementation(const FVector& PreviousFloorImpactNormal, const FVector& PreviousFloorContactNormal, const FVector& PreviousLocation, float TimeDelta) override;

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
