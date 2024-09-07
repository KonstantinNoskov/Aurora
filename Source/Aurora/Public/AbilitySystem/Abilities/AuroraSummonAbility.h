#pragma once

#include "CoreMinimal.h"
#include "AuroraGameplayAbility.h"
#include "AuroraSummonAbility.generated.h"

class AAuroraEnemy;

UCLASS()
class AURORA_API UAuroraSummonAbility : public UAuroraGameplayAbility
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocations();

	UFUNCTION(BlueprintPure, Category = "Summoning")
	const AAuroraEnemy* GetRandomMinionClass();

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	int32 NumMinions = 5;

	UPROPERTY(EditAnywhere, Category = "Summoning")
	TArray<TSubclassOf<AAuroraEnemy>> MinionClasses;

	UPROPERTY(EditAnywhere, Category = "Summoning")
	float MinSpawnDistance = 50.f;

	UPROPERTY(EditAnywhere, Category = "Summoning")
	float MaxSpawnDistance = 250.f;

	UPROPERTY(EditAnywhere, Category = "Summoning")
	float SpawnSpread = 90.f;
	
};
