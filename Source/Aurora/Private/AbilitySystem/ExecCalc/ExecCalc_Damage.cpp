#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuroraAttributeSet.h"

struct AuroraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)
	
	AuroraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuroraAttributeSet, Armor, Target, false);
	}
};
static const AuroraDamageStatics& DamageStatics()
{
	static AuroraDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// Source and Target ASC references
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	const AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	// Creating spec
	const FGameplayEffectSpec Spec = ExecutionParams.GetOwningSpec();

	// Gather tags from source and target spec's
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// Creating evaluation params and assign tags to it
	FAggregatorEvaluateParameters EvaluationParams;
	EvaluationParams.SourceTags = SourceTags;
	EvaluationParams.TargetTags = TargetTags;

	// Armor value container
	float ArmorMagnitude = 0.f;
	
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParams, ArmorMagnitude);

	// To insure armor >= 0.f
	ArmorMagnitude = FMath::Max<float>(ArmorMagnitude, 0.f);
	++ArmorMagnitude;

	const FGameplayModifierEvaluatedData EvaluatedData(DamageStatics().ArmorProperty, EGameplayModOp::Additive, ArmorMagnitude);
	
	// Returns output armor magnitude
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
