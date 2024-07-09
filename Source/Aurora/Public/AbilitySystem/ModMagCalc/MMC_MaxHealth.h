#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_BaseCalc.h"
#include "MMC_MaxHealth.generated.h"


UCLASS()
class AURORA_API UMMC_MaxHealth : public UMMC_BaseCalc
{
	GENERATED_BODY()

public:

	UMMC_MaxHealth();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:

	FGameplayEffectAttributeCaptureDefinition VigorDef;
};
