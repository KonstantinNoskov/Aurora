#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_BaseCalc.generated.h"


UCLASS()
class AURORA_API UMMC_BaseCalc : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

protected:

	void GetCalculationParams(
		const FGameplayEffectSpec& ParamSpec, const FGameplayEffectAttributeCaptureDefinition& AttributeDefinition, float& AttributeValue, int32& ActorLevel) const;

	
};
