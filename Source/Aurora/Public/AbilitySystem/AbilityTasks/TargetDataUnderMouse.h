#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);

UCLASS()
class AURORA_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidData;
	
	UFUNCTION(BlueprintCallable, Category = "Ability | Tasks",
		meta = (
			DisplayName = "TargetUnderMouse",
			HidePin = "OwningAbility",
			DefaultToSelf = "OwningAbility",
			BlueprintInternalUseOnly = "true"
			))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

protected:
	
	virtual void Activate() override;

private:
	
	void SendMouseCursorData();

	UFUNCTION()
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);
};
