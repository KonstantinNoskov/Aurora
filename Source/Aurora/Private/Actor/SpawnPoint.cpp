#include "Actor/SpawnPoint.h"

#include "Characters/AuroraEnemy.h"


ASpawnPoint::ASpawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	HandleSpawnEnemy();
}

void ASpawnPoint::HandleSpawnEnemy()	
{	
	// Valid Check
	if (!bShouldSpawn) return;
	
	// Spawn Params
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// Spawn Enemy
	SpawnedEnemy = GetWorld()->SpawnActor<AAuroraEnemy>(EnemySpawnClass, GetActorLocation(), GetActorRotation(), SpawnParams);
		
	if (IsValid(SpawnedEnemy))
	{
		SpawnedEnemy->GetOnDeathDelegate().AddDynamic(this, &ASpawnPoint::OnSpawnedEnemyDied);
	}
}

void ASpawnPoint::OnSpawnedEnemyDied(AActor* DeadActor)
{	
	bShouldSpawn = false;
}
