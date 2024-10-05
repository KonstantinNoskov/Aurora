#pragma once

#include "CoreMinimal.h"
#include "AuroraGameplayAbility.h"
#include "Interfaces/Interaction/CombatInterface.h"
#include "AuroraDamageGameplayAbility.generated.h"

struct FDamageEffectParams;


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

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FScalableFloat DamageValue;

#pragma region Debuff properties

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Debuff" )
	float DebuffChance = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Debuff" )
	float DebuffDamage = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Debuff" )
	float DebuffFrequency = 1.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Debuff" )
	float DebuffDuration = 5.f;
	
#pragma endregion
	
	
};
