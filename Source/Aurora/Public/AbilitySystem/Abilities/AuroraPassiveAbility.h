#pragma once

#include "CoreMinimal.h"
#include "AuroraGameplayAbility.h"
#include "AuroraPassiveAbility.generated.h"


UCLASS()
class AURORA_API UAuroraPassiveAbility : public UAuroraGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;


	void ReceiveDeactivate(const FGameplayTag& InAbilityTag);
};
