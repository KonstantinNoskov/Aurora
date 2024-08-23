#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "AuroraAbilitySystemGlobals.generated.h"


UCLASS()
class AURORA_API UAuroraAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
