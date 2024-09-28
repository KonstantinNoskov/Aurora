#pragma once

#include "CoreMinimal.h"
#include "AuroraDamageGameplayAbility.h"
#include "AuroraProjectileSpell.generated.h"

class AAuroraProjectile;

UCLASS()
class AURORA_API UAuroraProjectileSpell : public UAuroraDamageGameplayAbility
{
	GENERATED_BODY()

public:

	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

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
};
