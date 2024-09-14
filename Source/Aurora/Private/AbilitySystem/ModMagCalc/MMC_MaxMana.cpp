#include "AbilitySystem/ModMagCalc/MMC_MaxMana.h"

#include "AbilitySystem/AuroraAttributeSet.h"
#include "Interfaces/Interaction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	IntelligenceDef.AttributeToCapture = UAuroraAttributeSet::GetIntelligenceAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	/*// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParams;
	EvaluationParams.SourceTags = SourceTags;
	EvaluationParams.TargetTags = TargetTags;

	float Intelligence = 0.f;
	
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluationParams, Intelligence);
	Intelligence = FMath::Max<float>(Intelligence, 0.f);

	int32 PlayerLevel = 1;
	
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());
	}*/


	int32 PlayerLevel = 1;
	float IntelligenceValue;
	
	GetCalculationParams(Spec, IntelligenceDef, IntelligenceValue, PlayerLevel);

	return 50.f + IntelligenceValue * 1 * PlayerLevel;
}


