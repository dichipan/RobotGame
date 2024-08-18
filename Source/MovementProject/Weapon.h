// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FirstPersonCharacter.h"
#include "Weapon.generated.h"

UCLASS()
class MOVEMENTPROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float damage = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float rateOfFire = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool canAttack = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ammoMagazineCurrent = 8;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ammoMagazineMax = 8;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ammoReservedCurrent = 32;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ammoReservedMax = 32;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float crosshairAmountMax = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float crosshairAmountCurrent = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float crosshairDefaultOffset = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float verticalRecoilBottom = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float verticalRecoilTop = 0.01f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float horizontalRecoilBottom = -0.01f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float horizontalRecoilTop = 0.01f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float aimDistance = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float aimFOVMultiplier = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float aimSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AFirstPersonCharacter* owningPlayer;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Attack(AFirstPersonCharacter* player);

	UFUNCTION(BlueprintCallable)
	void Reload();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
