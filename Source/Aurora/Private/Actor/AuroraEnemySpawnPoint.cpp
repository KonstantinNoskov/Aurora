#include "Actor/AuroraEnemySpawnPoint.h"

#include "Characters/AuroraEnemy.h"

AAuroraEnemySpawnPoint::AAuroraEnemySpawnPoint()
{
	PrimaryActorTick.bCanEverTick = true;
}


void AAuroraEnemySpawnPoint::BeginPlay()
{
	Super::BeginPlay();
}

void AAuroraEnemySpawnPoint::SpawnEnemy()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AAuroraEnemy* Enemy = GetWorld()->SpawnActorDeferred<AAuroraEnemy>(EnemyClass, GetActorTransform());
	Enemy->SetLevel(EnemyLevel);
	Enemy->SetCharacterClass(CharacterClass);
	Enemy->FinishSpawning(GetActorTransform());
	Enemy->SpawnDefaultController();
}




