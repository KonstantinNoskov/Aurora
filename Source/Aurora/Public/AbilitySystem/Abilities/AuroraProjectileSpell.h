﻿#pragma once

#include "CoreMinimal.h"
#include "AuroraGameplayAbility.h"
#include "AuroraProjectileSpell.generated.h"

class AAuroraProjectile;

UCLASS()
class AURORA_API UAuroraProjectileSpell : public UAuroraGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData)
	override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AAuroraProjectile> ProjectileClass;
	
};