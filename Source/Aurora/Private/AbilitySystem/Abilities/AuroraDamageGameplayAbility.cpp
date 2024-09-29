#include "AbilitySystem/Abilities/AuroraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

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

float UAuroraDamageGameplayAbility::GetDamageByDamageType(float Inlevel, const FGameplayTag& DamageType)
{
	checkf(DamageTypes.Contains(DamageType), TEXT("[%hs]: GameplayAbility [%s] doesn't contain DamageType [%s]"), __FUNCTION__, *GetNameSafe(this), *DamageType.ToString())
	return DamageTypes[DamageType].GetValueAtLevel(Inlevel);
}

void UAuroraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	
	for (auto Pair : DamageTypes)
	{
		const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Pair.Key, ScaledDamage);
	}
	
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(),UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}
