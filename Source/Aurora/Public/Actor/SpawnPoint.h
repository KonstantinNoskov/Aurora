#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnPoint.generated.h"

class AAuroraEnemy;

UCLASS()
class AURORA_API ASpawnPoint : public AActor
{
	GENERATED_BODY()

public:

	ASpawnPoint();

protected:

	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable)
	void HandleSpawnEnemy();

protected:

	UFUNCTION()
	void OnSpawnedEnemyDied(AActor* DeadActor);

	UPROPERTY(EditInstanceOnly, Category = "Spawn Defaults")
	TSubclassOf<AAuroraEnemy> EnemySpawnClass;

	UPROPERTY(VisibleAnywhere, Category = "Spawn Defaults")
	TObjectPtr<AAuroraEnemy> SpawnedEnemy;

	UPROPERTY(EditInstanceOnly, SaveGame, Category = "Spawn Defaults")
	bool bShouldSpawn = true;

	
};
