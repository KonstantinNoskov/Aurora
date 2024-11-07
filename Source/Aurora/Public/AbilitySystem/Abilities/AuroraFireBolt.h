#pragma once

#include "CoreMinimal.h"
#include "AuroraProjectileSpell.h"
#include "AuroraFireBolt.generated.h"

UCLASS()
class AURORA_API UAuroraFireBolt : public UAuroraProjectileSpell
{
	GENERATED_BODY()

public:
	
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

	UFUNCTION(BlueprintCallable)
	void SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomingTarget);
	
	
};
