#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WaitCooldownChange.generated.h"

struct FGameplayTag;
class UAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCooldownChangeSignature, float, TimeRemaining);

UCLASS(BlueprintType, meta = (ExposedAsyncProxy = "AsyncTask"))
class AURORA_API UWaitCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintAssignable)
	FOnCooldownChangeSignature CooldownStart;

	UPROPERTY(BlueprintAssignable)
	FOnCooldownChangeSignature CooldownEnd;

	UFUNCTION(BlueprintCallable, meta =(BlueprintInternalUseOnly = "true"))
	static UWaitCooldownChange* WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTag);

	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	FGameplayTag CooldownTag;
	
	void CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount);

	void OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveEffectHandle);
	
};
