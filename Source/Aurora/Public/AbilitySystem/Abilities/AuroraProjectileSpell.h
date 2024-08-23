#pragma once

#include "CoreMinimal.h"
#include "AuroraDamageGameplayAbility.h"
#include "AuroraProjectileSpell.generated.h"

class AAuroraProjectile;

UCLASS()
class AURORA_API UAuroraProjectileSpell : public UAuroraDamageGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData)
	override;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AAuroraProjectile> ProjectileClass;

	
};
