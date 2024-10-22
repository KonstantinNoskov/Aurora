#include "AbilitySystem/Abilities/AuroraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuroraAbilityTypes.h"

FTaggedMontage UAuroraDamageGameplayAbility::GetRandomTaggedMontageFromArray(
	const TArray<FTaggedMontage>& TaggedMontages)
{
	if (TaggedMontages.Num() > 0)
	{
		const int32 MontageRandomIndex  = FMath::RandRange(0, TaggedMontages.Num() - 1);
		return TaggedMontages[MontageRandomIndex];
	}

	return FTaggedMontage();
}

/*float UAuroraDamageGameplayAbility::GetDamageByDamageType(float Inlevel, const FGameplayTag& DamageType)
{
	checkf(DamageTypes.Contains(DamageType), TEXT("[%hs]: GameplayAbility [%s] doesn't contain DamageType [%s]"), __FUNCTION__, *GetNameSafe(this), *DamageType.ToString())
	return DamageTypes[DamageType].GetValueAtLevel(Inlevel);
}*/

void UAuroraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	
	/*for (auto Pair : DamageTypes)
	{
		const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Pair.Key, ScaledDamage);
	}*/

	const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageType, ScaledDamage);
	
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

FDamageEffectParams UAuroraDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor) const
{
	FDamageEffectParams Params;
	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	Params.DamageGameplayEffectClass = DamageEffectClass;
	Params.SourceASC = GetAbilitySystemComponentFromActorInfo();
	Params.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	Params.BaseDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	Params.AbilityLevel = GetAbilityLevel();
	Params.DamageType = DamageType;
	Params.DebuffChance = DebuffChance;
	Params.DebuffDamage = DebuffDamage;
	Params.DebuffDuration = DebuffDuration;
	Params.DebuffFrequency = DebuffFrequency;
	Params.DeathImpulseMagnitude = DeathImpulseMagnitude;
	
	return Params; 
}
FProjectileBehaviorParams UAuroraDamageGameplayAbility::MakeProjectileBehaviorParams(AActor* InHomingActor, const FVector& InTargetLocation) const
{
	FProjectileBehaviorParams Params;

	Params.ActivationSpan = ProjectileActivationSpan;
	Params.bHoming = bHoming;
	Params.HomingActor = InHomingActor;
	Params.ProjectileTargetLocation = InTargetLocation;
	Params.HomingAccelerationMin = HomingAccelerationMin;
	Params.HomingAccelerationMax = HomingAccelerationMax;
	
	return Params;
}

float UAuroraDamageGameplayAbility::GetDamageAtLevel() const
{
	return  Damage.GetValueAtLevel(GetAbilityLevel());
}
