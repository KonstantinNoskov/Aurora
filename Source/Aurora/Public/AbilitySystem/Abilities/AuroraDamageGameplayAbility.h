#pragma once

#include "CoreMinimal.h"
#include "AuroraGameplayAbility.h"
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

public:

	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);
};
