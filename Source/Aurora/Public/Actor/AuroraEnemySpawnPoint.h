#pragma once

#include "CoreMinimal.h"
#include "Characters/AuroraCharacterBase.h"
#include "Engine/TargetPoint.h"
#include "AuroraEnemySpawnPoint.generated.h"

class AAuroraEnemy;

UCLASS()
class AURORA_API AAuroraEnemySpawnPoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	AAuroraEnemySpawnPoint();

protected:

	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Class")
	TSubclassOf<AAuroraEnemy> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Class")
	int32  EnemyLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Class")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

};
