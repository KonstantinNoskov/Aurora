#include "AbilitySystem/ModMagCalc/MMC_BaseCalc.h"

#include "Interfaces/Interaction/CombatInterface.h"

void UMMC_BaseCalc::GetCalculationParams(
	
	const FGameplayEffectSpec& ParamSpec, const FGameplayEffectAttributeCaptureDefinition& AttributeDefinition, float& AttributeValue, int32& ActorLevel) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = ParamSpec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = ParamSpec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParams;
	EvaluationParams.SourceTags = SourceTags;
	EvaluationParams.TargetTags = TargetTags;
	
	GetCapturedAttributeMagnitude(AttributeDefinition, ParamSpec, EvaluationParams, AttributeValue);
	AttributeValue = FMath::Max<float>(AttributeValue, 0.f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(ParamSpec.GetContext().GetSourceObject());

	if (ParamSpec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		ActorLevel = ICombatInterface::Execute_GetPlayerLevel(ParamSpec.GetContext().GetSourceObject());	
	}
	
	
}
