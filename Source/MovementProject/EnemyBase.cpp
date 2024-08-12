// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyBase.h"
#include "RoundHandler.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = false;
	handler = nullptr;
	spawner = nullptr;
	targetPlayer = nullptr;

}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = speed;
	SpawnDefaultController();

}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//if (targetPlayer && DistanceToPlayer() < attackDistance && !isAttacking) {
		//isAttacking = true;
		//Attack();
		//GetWorld()->GetTimerManager().SetTimer(handle, this, &AEnemyBase::ResetAttack, attackCooldown);
	//}
	//if (targetPlayer) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%f"), DistanceToPlayer()));

}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyBase::AILoop()
{
	if (targetPlayer && DistanceToPlayer() < attackDistance && !isAttacking) {
		isAttacking = true;
		Attack();
		FTimerHandle attackHandle;
		GetWorld()->GetTimerManager().SetTimer(attackHandle, this, &AEnemyBase::ResetAttack, attackCooldown);
	}
	GetWorld()->GetTimerManager().SetTimer(handle, this, &AEnemyBase::AILoop, 0.1f);
}

// Called once an enemy is spawned
void AEnemyBase::Initialize(ARoundHandler* roundHandler, AEnemySpawner* enemySpawner) {
	handler = roundHandler;
	handler->SpawnSuccess();
	spawner = enemySpawner;
	SetHealth(handler->round);
	AILoop();
}

// Sets health based on the round
void AEnemyBase::SetHealth(int round)
{
	if (round < 10)
	{
		health = health + 100.0f;
	}
	else
	{
		health *= 1.1f;
	}
}

// Resets attack so enemy may hit again
void AEnemyBase::ResetAttack()
{
	isAttacking = false;
}

// Returns distance to target player
float AEnemyBase::DistanceToPlayer()
{
	return FVector::Dist(this->GetActorLocation(), targetPlayer->GetActorLocation());
}

// Called once health reaches 0
void AEnemyBase::Death() {
	if (isAlive) {
		isAlive = false;
		handler->EnemyDeath();
	}
}
