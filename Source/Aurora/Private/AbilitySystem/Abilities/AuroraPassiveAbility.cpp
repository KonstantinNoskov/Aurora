#include "AbilitySystem/Abilities/AuroraPassiveAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuroraAbilitySystemComponent.h"

void UAuroraPassiveAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
	)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UAuroraAbilitySystemComponent* AuroraASC = Cast<UAuroraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		AuroraASC->OnPassiveAbilityDeactivated.AddUObject(this, &UAuroraPassiveAbility::ReceiveDeactivate);
	}
}

void UAuroraPassiveAbility::ReceiveDeactivate(const FGameplayTag& InAbilityTag)
{
	if (AbilityTags.HasTagExact(InAbilityTag))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}
