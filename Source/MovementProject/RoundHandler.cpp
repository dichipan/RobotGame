// Fill out your copyright notice in the Description page of Project Settings.


#include "RoundHandler.h"
#include "Runtime/Engine/Public/TimerManager.h"

// Sets default values
ARoundHandler::ARoundHandler()
{

}

// Called when the game starts or when spawned
void ARoundHandler::BeginPlay()
{
	Super::BeginPlay();
	RoundStart();
}

// Begin a new round
void ARoundHandler::RoundStart() {
	round++;
	enemiesToSpawn = (5 + round * 1.5f);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Round %d"), round));
	GetWorld()->GetTimerManager().SetTimer(handle, this, &ARoundHandler::RoundLoop, 7.0f);
}

// Spawn correct amount of zombies at start of round
void ARoundHandler::RoundLoop() {
	if (enemiesToSpawn > 0 && enemiesSpawned < enemyCap) {
		SpawnEnemy();
		GetWorld()->GetTimerManager().SetTimer(handle, this, &ARoundHandler::RoundLoop, 1.0f);
	}
}

// Spawn an enemy
void ARoundHandler::SpawnEnemy() {
	if (enemiesToSpawn > 0) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Spawning Enemy, %d"), enemiesToSpawn));
		TSubclassOf<AEnemyBase> enemy = Enemies[FMath::RandRange(0, Enemies.Num() - 1)];

		AEnemySpawner* spawnPoint = EnemySpawners[FMath::RandRange(0, EnemySpawners.Num() - 1)];

		//FRotator Rotation(0.0f, 0.0f, 0.0f); LATER ON CHANGE TO LOOKAT ROTATION FROM SPAWNPOINT->SPAWNLOCATION WORLD LOCATION TO SPAWNPOINT WORLD LOCATION

		AEnemyBase* spawnedEnemy = GetWorld()->SpawnActor<AEnemyBase>(enemy, spawnPoint->SpawnLocation, FRotator(0.0f, 0.0f, 0.0f));
		if(spawnedEnemy) spawnedEnemy->Initialize(this, spawnPoint);
	}
}

// Handle spawned count when enemy death occurs
void ARoundHandler::EnemyDeath() {
	enemiesSpawned--;
	if (enemiesSpawned <= 0) {
		GetWorld()->GetTimerManager().SetTimer(handle, this, &ARoundHandler::RoundStart, 3.0f);
	}
	else {
		SpawnEnemy();
	}
}

// Called if enemy spawns successfully
void ARoundHandler::SpawnSuccess() {
	enemiesToSpawn--;
	enemiesSpawned++;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("HELLO I HAVE SPAWNED")));
}

