#pragma once

#include "CoreMinimal.h"
#include "AuroraGameplayTags.h"
#include "AuroraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnSpellGlobeSelectedSignature, bool, bSpendPointsButtonEnabled, bool, bEquipButtonEnabled, FString, DescriptionString, FString, NextLevelDescriptionString);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitForEquipSelectionSignature, const FGameplayTag&, AbilityTypeTag);


struct FAuroraGameplayTags;


struct FSelectedAbility
{
	FGameplayTag AbilityTag = FGameplayTag();
	FGameplayTag StatusTag = FGameplayTag();
};

UCLASS(BlueprintType, Blueprintable)
class AURORA_API USpellMenuWidgetController : public UAuroraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	// Delegates
	UPROPERTY(BlueprintAssignable)
	FOnSpellGlobeSelectedSignature OnSpellGlobeSelected;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature SpellPointsUpdated;

	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature WaitForEquip;

	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature StopWaitingForEquip;


private:

	static void ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints, bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton);
	
	UFUNCTION(BlueprintCallable)
	void SpellGlobeSelected(const FGameplayTag& InAbilityTag);

	UFUNCTION(BlueprintCallable)
	void SpendPointButtonPressed();

public:

	UFUNCTION(BlueprintCallable)
	void GlobeDeselect();

	UFUNCTION(BlueprintCallable)
	void EquipButtonPressed();

private:

	FSelectedAbility SelectedAbility = {FAuroraGameplayTags::Get().Abilities_None, FAuroraGameplayTags::Get().Abilities_Status_Locked };
	int32 CurrentSpellPoints = 0;
	bool bWaitingForEquipSelection = false;
	
};
