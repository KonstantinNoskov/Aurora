﻿#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AuroraGameplayTags.h"
#include "AbilitySystem/AuroraAbilitySystemLibrary.h"
#include "AbilitySystem/AuroraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interfaces/Interaction/CombatInterface.h"

struct AuroraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration)
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage)
	
	AuroraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuroraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuroraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuroraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuroraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuroraAttributeSet, CriticalHitResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuroraAttributeSet, CriticalHitDamage, Source, false);
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
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// Source references
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	const UCharacterClassInfo* SourceCharacterClassInfo = UAuroraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	
	// Target references
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);
	const UCharacterClassInfo* TargetCharacterClassInfo = UAuroraAbilitySystemLibrary::GetCharacterClassInfo(TargetAvatar);
	
	// Creating spec
	const FGameplayEffectSpec Spec = ExecutionParams.GetOwningSpec();

	// Gather tags from source and target spec's
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// Creating evaluation params and assign tags to it
	FAggregatorEvaluateParameters EvaluationParams;
	EvaluationParams.SourceTags = SourceTags;
	EvaluationParams.TargetTags = TargetTags;

#pragma region Calculate damage
	
	// Get Damage Set by Caller Magnitude
	float Damage = Spec.GetSetByCallerMagnitude(FAuroraGameplayTags::Get().Damage);

#pragma endregion
#pragma region Calculate block chance 
	
	// Capture BlockChance on Target, and determine if there was a successful Block
	// If Block was successful, halve the damage
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef , EvaluationParams, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f); 

	const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance;
	Damage = bBlocked ? Damage * .5f : Damage;

#pragma endregion
#pragma region Calculate armor penetration
	
	// Capture Target Armor handle
	// Armor penetration ignores a percentage of the Target's armor
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef , EvaluationParams, TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f);

	// Capture Source Armor penetration handle
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef , EvaluationParams, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.f);
	
	// Getting a armor penetration coefficient value from a CharacterClassInfo
	const FRealCurve* ArmorPenetrationCurve = SourceCharacterClassInfo->AttributesCalcCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceCombatInterface->GetPlayerLevel());

	// Getting a effective armor coefficient value from a CharacterClassInfo
	const FRealCurve* EffectiveArmorCurve = SourceCharacterClassInfo->AttributesCalcCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetCombatInterface->GetPlayerLevel());
	
	/*	
	 *	Effective Armor = Target Armor after applying Source Armor penetration.
	 *	The lower coefficient the lower the damage
	 *
	 *	Example:
	 *	Target Armor = 100;
	 *	Source Armor penetration = 50;
	 *	Armor penetration coefficient = .75;
	 *
	 *	Effective Armor = 80 * ((100 - 50 * .75) / 100) = 50 
	 */
	const float EffectiveArmor = TargetArmor *= (100 - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100;
	
	/* 
	 *	Damage = Effective Armor * Effective Armor coefficient
	 *
	 *	Example:
	 *	Damage = 20;
	 *	Effective Armor = 50;
	 *	Effective Armor coefficient = .3;
	 *
	 *	Damage = 20 * ((100 - 50 * .3) / 100) = 17;
	 */
	Damage *= (100 - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;


#pragma endregion
#pragma region Calculate Critical hit chance

	// Capture Source critical hit chance
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef , EvaluationParams, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.f);

	// Capture Target critical hit resistance
	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef , EvaluationParams, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance, 0.f);

	// Capture Target critical hit resistance
	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluationParams, TargetCriticalHitResistance);
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.f);

	// Getting a critical hit multiplier coefficient value from a CharacterClassInfo
	const FRealCurve* CriticalHitMultiplierCurve = SourceCharacterClassInfo->AttributesCalcCoefficients->FindCurve(FName("CriticalHitMultiplier"), FString());
	const float CriticalHitCoefficient = CriticalHitMultiplierCurve->Eval(SourceCombatInterface->GetPlayerLevel());
	
	/*
	 * CriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance
	 * CriticalDamage = Damage * 2 + SourceCriticalHitDamage
	 */
	const float EffectiveCriticalChance = SourceCriticalHitChance - TargetCriticalHitResistance /** 0.15f*/;
	const bool bCriticalHit = FMath::RandRange(1, 100) < EffectiveCriticalChance;
	Damage = bCriticalHit ? CriticalHitCoefficient * Damage: Damage;
	
#pragma endregion
	
	const FGameplayModifierEvaluatedData EvaluatedData(UAuroraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	
	// Returns output armor magnitude
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}