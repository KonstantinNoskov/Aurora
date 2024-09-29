#pragma once

#include "CoreMinimal.h"
#include "AuroraGameplayAbility.h"
#include "Interfaces/Interaction/CombatInterface.h"
#include "AuroraDamageGameplayAbility.generated.h"




UCLASS()
class AURORA_API UAuroraDamageGameplayAbility : public UAuroraGameplayAbility
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;

	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages);

	float GetDamageByDamageType(float Inlevel, const FGameplayTag& DamageType);

public:

	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);
};
