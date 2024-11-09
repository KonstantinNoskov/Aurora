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
	void SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch = false, float PitchOverride = 0.f);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AAuroraProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	int32 ProjectilesNum = 5;

	/* Enable projectile pitch override at spawn.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Behavior")
	bool bPitchOverride = false;

	/* Adjusting projectile pitch angle at spawn.
	 * 90 == UpVector
	 * -90 == DownVector
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Behavior", meta = (EditCondition = "bPitchOverride"))
	float PitchAdjustment = 0.f;
};
