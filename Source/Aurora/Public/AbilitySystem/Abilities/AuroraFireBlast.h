#pragma once

#include "CoreMinimal.h"
#include "AuroraDamageGameplayAbility.h"
#include "AuroraFireBlast.generated.h"

class AAuroraFireBall;

UCLASS()
class AURORA_API UAuroraFireBlast : public UAuroraDamageGameplayAbility
{
	GENERATED_BODY()

public:
	
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

	UFUNCTION(BlueprintCallable)
	TArray<AAuroraFireBall*> SpawnFireBalls();

protected:

	UPROPERTY(EditDefaultsOnly)
	int32 NumFireBalls = 12;
	
private:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAuroraFireBall> FireBallClass;
	
};
