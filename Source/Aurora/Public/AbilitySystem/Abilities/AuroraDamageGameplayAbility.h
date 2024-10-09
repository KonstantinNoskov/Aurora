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
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr) const;

	UFUNCTION(BlueprintCallable)
	FProjectileBehaviorParams MakeProjectileBehaviorParams() const;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FScalableFloat Damage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DeathImpulseMagnitude = 60.f;

	

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
