// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called to set new ammo, assumes ammoReserveCurrent > 0
void AWeapon::Reload()
{
	if (ammoReservedCurrent > 0) {
		int ammoToReload = ammoMagazineMax - ammoMagazineCurrent;
		if (ammoReservedCurrent >= ammoToReload) {
			ammoReservedCurrent -= ammoToReload;
			ammoMagazineCurrent = ammoMagazineMax;
		}
		else {
			ammoMagazineCurrent += ammoReservedCurrent;
			ammoReservedCurrent = 0;
		}
	}
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

