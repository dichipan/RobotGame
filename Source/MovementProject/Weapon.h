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
	AFirstPersonCharacter* owningPlayer;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Attack(AFirstPersonCharacter* player);

	UFUNCTION(BlueprintCallable)
	void Reload();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
