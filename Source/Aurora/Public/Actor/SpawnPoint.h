#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "Interfaces/SaveInterface.h"
#include "SpawnPoint.generated.h"

class UCapsuleComponent;
class AAuroraEnemy;
class UTexture2D;

UCLASS()
class AURORA_API ASpawnPoint : public AActor, public ISaveInterface
{
	GENERATED_BODY()

public:

	ASpawnPoint(const FObjectInitializer& ObjectInitializer);

protected:

	virtual void BeginPlay() override;

public:

#if WITH_EDITOR
	
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
#endif

	UFUNCTION(BlueprintCallable)
	void HandleSpawnEnemy();

protected:

	UFUNCTION()
	void OnSpawnedEnemyDied(AActor* DeadActor);
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> Root;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBillboardComponent> BillBoard;

	UPROPERTY()
	TObjectPtr<UTexture2D> InitialSprite;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> PreviewMesh;

	UPROPERTY(EditInstanceOnly, Category = "Spawn Defaults")
	TSubclassOf<AAuroraEnemy> EnemySpawnClass;

	UPROPERTY(VisibleAnywhere, Category = "Spawn Defaults")
	TObjectPtr<AAuroraEnemy> SpawnedEnemy;

	UPROPERTY(EditInstanceOnly, SaveGame, Category = "Spawn Defaults")
	bool bShouldSpawn = true;

	
};
