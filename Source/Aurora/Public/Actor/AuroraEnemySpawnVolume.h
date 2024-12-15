#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/SaveInterface.h"
#include "AuroraEnemySpawnVolume.generated.h"

class AAuroraEnemySpawnPoint;
class UBoxComponent;

UCLASS()
class AURORA_API AAuroraEnemySpawnVolume : public AActor, public ISaveInterface
{
	GENERATED_BODY()

public:
	
	AAuroraEnemySpawnVolume();
	
protected:
	
	virtual void BeginPlay() override;

public:

	virtual void LoadActor_Implementation() override;

private:

	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> Box;

	UPROPERTY(EditAnywhere)
	TArray<AAuroraEnemySpawnPoint*> SpawnPoints;

protected:
	
	UPROPERTY(BlueprintReadOnly, SaveGame)
	bool bReached = false;
	
};
