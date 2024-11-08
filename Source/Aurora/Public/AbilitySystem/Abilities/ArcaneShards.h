#pragma once

#include "CoreMinimal.h"
#include "AuroraDamageGameplayAbility.h"
#include "ArcaneShards.generated.h"

UCLASS()
class AURORA_API UArcaneShards : public UAuroraDamageGameplayAbility
{
	GENERATED_BODY()
	
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxNumShards = 11;
};
