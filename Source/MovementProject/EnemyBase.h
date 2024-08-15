#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemySpawner.h"
#include "FirstPersonCharacter.h"
#include "EnemyBase.generated.h"

class ARoundHandler;

UCLASS()
class MOVEMENTPROJECT_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

	ARoundHandler* handler;

	FTimerHandle handle;

	UPROPERTY(BlueprintReadWrite)
		AEnemySpawner* spawner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AFirstPersonCharacter* targetPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool inMap = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isAlive = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isAttacking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float health = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float damage = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float speed = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float attackDistance = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float attackCooldown = 1.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void AILoop();

	void Initialize(ARoundHandler* roundHandler, AEnemySpawner* enemySpawner);

	void SetHealth(int round);

	void ResetAttack();

	UFUNCTION(BlueprintCallable)
		float DistanceToPlayer();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void ChasePlayer();

	UFUNCTION(BlueprintCallable)
		void Death();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void Attack();

};
