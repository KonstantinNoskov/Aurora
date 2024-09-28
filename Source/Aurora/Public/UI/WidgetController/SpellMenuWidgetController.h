#pragma once

#include "CoreMinimal.h"
#include "AuroraGameplayTags.h"
#include "AuroraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"


struct FAuroraGameplayTags;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnSpellGlobeSelectedSignature, bool, bSpendPointsButtonEnabled, bool, bEquipButtonEnabled, FString, DescriptionString, FString, NextLevelDescriptionString);

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

private:

	static void ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints, bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton);
	
	UFUNCTION(BlueprintCallable)
	void SpellGlobeSelected(const FGameplayTag& InAbilityTag);

	UFUNCTION(BlueprintCallable)
	void SpendPointButtonPressed();

	FSelectedAbility SelectedAbility = {FAuroraGameplayTags::Get().Abilities_None, FAuroraGameplayTags::Get().Abilities_Status_Locked };
	int32 CurrentSpellPoints = 0;
	
};
