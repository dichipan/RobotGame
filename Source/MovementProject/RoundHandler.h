#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBase.h"
#include "EnemySpawner.h"
#include "RoundHandler.generated.h"


UCLASS()
class MOVEMENTPROJECT_API ARoundHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoundHandler();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int round = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int enemiesToSpawn = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int enemiesSpawned = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int enemyCap = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float spawnDelay = 1.75f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float roundStartDelay = 7.0f;

	FTimerHandle handle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void RoundStart();
	void RoundLoop();
	void SpawnEnemy();
	void EnemyDeath();
	void SpawnSuccess();

	UPROPERTY(EditDefaultsOnly, Category = "Enemy");
		TArray <TSubclassOf<AEnemyBase>> Enemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AEnemySpawner*> EnemySpawners;
};
