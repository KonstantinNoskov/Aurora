#include "AbilitySystem/AuroraAbilitySystemGlobals.h"

#include "AuroraAbilityTypes.h"

FGameplayEffectContext* UAuroraAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FAuroraGameplayEffectContext();
}
