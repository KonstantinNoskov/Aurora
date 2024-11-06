#pragma once

#include "CoreMinimal.h"
#include "AuroraGameplayAbility.h"
#include "Interfaces/Interaction/CombatInterface.h"
#include "AuroraDamageGameplayAbility.generated.h"

struct FDamageEffectParams;
struct FProjectileBehaviorParams; 


UCLASS()
class AURORA_API UAuroraDamageGameplayAbility : public UAuroraGameplayAbility
{
	GENERATED_BODY()

protected:
	
	// Used to handle multiple damage types
	
	/*UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;

	float GetDamageByDamageType(float InLevel, const FGameplayTag& DamageType);
	*/

	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages);

public:

	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr, FVector InRadialDamageOrigin = FVector::ZeroVector) const;

	UFUNCTION(BlueprintCallable)
	FProjectileBehaviorParams MakeProjectileBehaviorParams(AActor* InHomingActor, const FVector& InTargetLocation) const;

	UFUNCTION(BlueprintCallable)
	float GetDamageAtLevel() const ;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FScalableFloat Damage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DeathImpulseMagnitude = 60.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	bool bIsRadialDamage = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float RadialDamageInnerRadius = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float RadialDamageOuterRadius = 0.f;


#pragma region Projectile Behavior Params

	UPROPERTY(EditDefaultsOnly, Category = " Projectile Behavior")
	float ProjectileSpread = 90.f;

	UPROPERTY(EditDefaultsOnly, Category = " Projectile Behavior")
	float HomingAccelerationMin = 1600.f;

	UPROPERTY(EditDefaultsOnly, Category = " Projectile Behavior")
	float HomingAccelerationMax = 3200.f;

#pragma endregion
#pragma region Debuff properties

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Debuff" )
	float DebuffChance = 13.f;

	UPROPERTY(EditDefaultsOnly, Category = "Debuff" )
	float DebuffDamage = 13.f;

	UPROPERTY(EditDefaultsOnly, Category = "Debuff" )
	float DebuffFrequency = 1.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Debuff" )
	float DebuffDuration = 13.f;
	
#pragma endregion
	
	
};
