#pragma once

#include "CoreMinimal.h"
#include "AuroraBeamSpell.h"
#include "ShockWave.generated.h"

UCLASS()
class AURORA_API UShockWave : public UAuroraBeamSpell
{
	GENERATED_BODY()
	
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
};
