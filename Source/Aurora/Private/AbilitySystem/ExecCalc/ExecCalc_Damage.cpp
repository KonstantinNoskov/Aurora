﻿#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AuroraAbilityTypes.h"
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
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance)
	
	AuroraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuroraAttributeSet, Armor,						Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuroraAttributeSet, BlockChance,				Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuroraAttributeSet, ArmorPenetration,			Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuroraAttributeSet, CriticalHitChance,			Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuroraAttributeSet, CriticalHitResistance,		Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuroraAttributeSet, CriticalHitDamage,			Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuroraAttributeSet, FireResistance,			Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuroraAttributeSet, LightningResistance,		Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuroraAttributeSet, ArcaneResistance,			Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuroraAttributeSet, PhysicalResistance,		Target, false);
		
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

	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;

	const FAuroraGameplayTags& Tags = FAuroraGameplayTags::Get();
		
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Armor,					DamageStatics().ArmorDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_ArmorPenetration,		DamageStatics().ArmorPenetrationDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_BlockChance,			DamageStatics().BlockChanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitChance,		DamageStatics().CriticalHitChanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitResistance,	DamageStatics().CriticalHitResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitDamage,		DamageStatics().CriticalHitDamageDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire,					DamageStatics().FireResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Lightning,				DamageStatics().LightningResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Arcane,				DamageStatics().ArcaneResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical,				DamageStatics().PhysicalResistanceDef);
	
	// Source/Target Ability Systems
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	// Source/Target Avatars
	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	// Source/Target Avatar's Levels
	int32 SourcePlayerLevel = 1;
	if (SourceAvatar->Implements<UCombatInterface>()) { SourcePlayerLevel = ICombatInterface::Execute_GetPlayerLevel(SourceAvatar); }

	int32 TargetPlayerLevel = 1;
	if (TargetAvatar->Implements<UCombatInterface>()) { TargetPlayerLevel = ICombatInterface::Execute_GetPlayerLevel(TargetAvatar); }

	// Source/Target Character Class Info
	const UCharacterClassInfo* SourceCharacterClassInfo = UAuroraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	const UCharacterClassInfo* TargetCharacterClassInfo = UAuroraAbilitySystemLibrary::GetCharacterClassInfo(TargetAvatar);
	
	// Creating spec & Context
	const FGameplayEffectSpec Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

	// Gather tags from source and target spec's
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();		

	// Creating evaluation params and assign tags to it
	FAggregatorEvaluateParameters EvaluationParams;
	EvaluationParams.SourceTags = SourceTags;
	EvaluationParams.TargetTags = TargetTags;
	
	// Debuff
	DetermineDebuff(ExecutionParams, Spec, EvaluationParams, TagsToCaptureDefs);
	
#pragma region Calculate damage resistance
	
	// Get Damage Set by Caller Magnitude
	float Damage = 0.0f;

	// Calculate damage amount for each damage type that was applied by Ability.  
	for (const TTuple<FGameplayTag, FGameplayTag>& Pair : FAuroraGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;
		
		checkf(TagsToCaptureDefs.Contains(ResistanceTag), TEXT("[%hs]: TagsToCaptureDefs doesn't contain Tag: [%s] in ExecCalc_Damage"), __FUNCTION__, *ResistanceTag.ToString());
		
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = TagsToCaptureDefs[ResistanceTag];

		float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTypeTag, false);

		// If damage <= 0, skip to the next damage type calculation
		if (DamageTypeValue <= 0.f)
		{
			continue;
		}
		
		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluationParams, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f); 
		
		DamageTypeValue *= (100.f - Resistance) / 100.f;
		Damage += DamageTypeValue;
		
		// Handle Radial damage calculation if needed
		if (UAuroraAbilitySystemLibrary::IsRadialDamage(EffectContextHandle))
		{
			Damage = CalculateRadialDamage(EffectContextHandle, Damage, TargetAvatar);
			
			/*if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(TargetAvatar))
			{
				
				CombatInterface->GetOnDamageSignature().AddLambda([&](float InDamageAmount)
				{
					DamageTypeValue = InDamageAmount;
					
				});
			}

			UGameplayStatics::ApplyRadialDamageWithFalloff(
				TargetAvatar,
				DamageTypeValue,
				0.f,
				UAuroraAbilitySystemLibrary::GetRadialDamageOrigin(EffectContextHandle),
				UAuroraAbilitySystemLibrary::GetRadialDamageInnerRadius(EffectContextHandle),
				UAuroraAbilitySystemLibrary::GetRadialDamageOuterRadius(EffectContextHandle),
				1.f,
				UDamageType::StaticClass(),
				TArray<AActor*>(),
				SourceAvatar,
				nullptr
				);*/
		}
	}
	
#pragma endregion
#pragma region Calculate block chance 
	
	// Capture BlockChance on Target, and determine if there was a successful Block
	// If Block was successful, halve the damage
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef , EvaluationParams, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f); 

	// Randomize the block chance
	const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance;

	// Inform gameplay effect context whether damage was blocked
	UAuroraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);
	
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
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourcePlayerLevel);

	// Getting a effective armor coefficient value from a CharacterClassInfo
	const FRealCurve* EffectiveArmorCurve = SourceCharacterClassInfo->AttributesCalcCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetPlayerLevel);
	
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
#pragma region Calculate critical hit chance

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
	const float CriticalHitCoefficient = CriticalHitMultiplierCurve->Eval(SourcePlayerLevel);
	
	/*
	 * CriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance
	 * CriticalDamage = Damage * 2 + SourceCriticalHitDamage
	 */
	const float EffectiveCriticalChance = SourceCriticalHitChance - TargetCriticalHitResistance /** 0.15f*/;
	const bool bCriticalHit = FMath::RandRange(1, 100) < EffectiveCriticalChance;

	// Inform gameplay effect context whether damage was critical
	UAuroraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);
	
	Damage = bCriticalHit ? CriticalHitCoefficient * Damage: Damage;
	
#pragma endregion
	
	const FGameplayModifierEvaluatedData EvaluatedData(UAuroraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	
	// Returns final damage value
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}

void UExecCalc_Damage::DetermineDebuff(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	const FGameplayEffectSpec& Spec, 
	FAggregatorEvaluateParameters EvaluationParams,
	const TMap <FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToCaptureDefs) const
{
	// Aurora gameplay tags
	const FAuroraGameplayTags GameplayTags = FAuroraGameplayTags::Get(); 
	
	for (TTuple<FGameplayTag, FGameplayTag> Pair : GameplayTags.DamageTypesToDebuffs)
	{
		const FGameplayTag& DamageType = Pair.Key;
		const FGameplayTag& DebuffType = Pair.Value;
		
		float TypeDamage = Spec.GetSetByCallerMagnitude(DamageType, false, -1.f);

		if (TypeDamage > -1.f)
		{
			// Determine if there was a successful debuff
			const float SourceDebuffChance = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Chance, false, -1.f);

			float TargetDebuffResistance = 0.f;
			const FGameplayTag ResistanceTag = GameplayTags.DamageTypesToResistances[DamageType];
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(InTagsToCaptureDefs[ResistanceTag], EvaluationParams, TargetDebuffResistance);
		
			TargetDebuffResistance = FMath::Max<float>(TargetDebuffResistance, 0.f);
			const float EffectiveDebuffChance = SourceDebuffChance * (100 - TargetDebuffResistance) / 100.f;
			const bool bDebuff = FMath::RandRange(1, 100) < EffectiveDebuffChance;

			if (bDebuff)
			{
				// Local storage ContextHandle from our GameplayEffectSpecHandle
				FGameplayEffectContextHandle ContextHandle = Spec.GetContext();

				// Assign debuff status and DebuffDamageType to our GameplayEffectSpecHandle
				UAuroraAbilitySystemLibrary::SetIsDebuffSuccessfull(ContextHandle, true);
				UAuroraAbilitySystemLibrary::SetDamageType(ContextHandle, DamageType);

				// Assign debuff damage, duration and frequency to our GameplayEffectSpecHandle
				const float DebuffDamage = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Damage, false, -1.f);
				const float DebuffDuration = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Duration, false, -1.f);
				const float DebuffFrequency = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Frequency, false, -1.f);

				UAuroraAbilitySystemLibrary::SetDebuffDamage(ContextHandle, DebuffDamage);
				UAuroraAbilitySystemLibrary::SetDebuffDuration(ContextHandle, DebuffDuration);
				UAuroraAbilitySystemLibrary::SetDebuffFrequency(ContextHandle, DebuffFrequency);
				
			}
		}
	}
}

float UExecCalc_Damage::CalculateRadialDamage(FGameplayEffectContextHandle&  InEffectContextHandle, const float InDamage, const AActor* InTargetAvatar) const 
{
	FVector TargetLocation = InTargetAvatar->GetActorLocation();
	const FVector Origin = UAuroraAbilitySystemLibrary::GetRadialDamageOrigin(InEffectContextHandle);

	// TargetAvatar halfheight may be above the InnerRadius
	TargetLocation.Z = Origin.Z; 
    
	const float SquareDistance = FVector::DistSquared(TargetLocation, Origin);
    
	const float InnerRadius = UAuroraAbilitySystemLibrary::GetRadialDamageInnerRadius(InEffectContextHandle);
	const float SquareInnerRadius = FMath::Square(InnerRadius);
 
	const float OuterRadius = UAuroraAbilitySystemLibrary::GetRadialDamageOuterRadius(InEffectContextHandle);
	const float SquareOuterRadius = FMath::Square(OuterRadius);
    
	if (SquareDistance <= SquareInnerRadius) return InDamage;
 
	const TRange<float> DistanceRange(SquareInnerRadius, SquareOuterRadius);
	const TRange<float> DamageScaleRange(1.0f, .1f);
	const float DamageScale = FMath::GetMappedRangeValueClamped(DistanceRange, DamageScaleRange, SquareDistance);
	const float RadialDamage = InDamage * DamageScale;
 
	return RadialDamage;
}