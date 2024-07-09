#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_BaseCalc.h"
#include "MMC_MaxMana.generated.h"


UCLASS()
class AURORA_API UMMC_MaxMana : public UMMC_BaseCalc
{
	GENERATED_BODY()

public:
	
	UMMC_MaxMana();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;


	
private:

	FGameplayEffectAttributeCaptureDefinition IntelligenceDef;

	
};
