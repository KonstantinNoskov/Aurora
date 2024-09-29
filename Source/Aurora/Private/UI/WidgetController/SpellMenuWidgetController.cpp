#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AuroraGameplayTags.h"
#include "AbilitySystem/AuroraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuroraPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();

	// Broadcast spell points changes to widgets
	SpellPointsUpdated.Broadcast(GetAuroraPS()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	// Ability status change
	GetAuroraASC()->AbilityStatusChanged.AddLambda(
		[this](const FGameplayTag& AbilityTag, const FGameplayTag& AbilityStatusTag, int32 NewLevel)
		{
			// #290
			if (SelectedAbility.AbilityTag.MatchesTagExact(AbilityTag))
			{	
				SelectedAbility.StatusTag = AbilityStatusTag;

				// Local storages
				bool bEnableSpendPoints = false;
				bool bEnableEquip = false;
				FString Description;
				FString NextLevelDescription;
				
				GetAuroraASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
				ShouldEnableButtons(AbilityStatusTag, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
				OnSpellGlobeSelected.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
			}
			
			if (AbilityInfo)
			{
				FAuroraAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
				Info.AbilityStatusTag = AbilityStatusTag;

				AbilityInfoDelegate.Broadcast(Info);
			}
		});

	// Spell Point Change
	GetAuroraPS()->OnSpellPointsChanged.AddLambda([this](int32 SpellPoints)
	{
		SpellPointsUpdated.Broadcast(SpellPoints);
		CurrentSpellPoints = SpellPoints;

		// #290
		bool bEnableSpendPoints = false;
		bool bEnableEquip = false;
		FString Description;
		FString NextLevelDescription;
				
		GetAuroraASC()->GetDescriptionsByAbilityTag(SelectedAbility.AbilityTag, Description, NextLevelDescription);
		ShouldEnableButtons(SelectedAbility.StatusTag, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
		OnSpellGlobeSelected.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
	});
}
void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& InAbilityTag)
{
	const int32 SpellPoints = GetAuroraPS()->GetSpellPoints();
	const FAuroraGameplayTags AuroraGameplayTags = FAuroraGameplayTags::Get();  
	const bool bTagValid = InAbilityTag.IsValid();
	const bool bTagNone = InAbilityTag.MatchesTag(AuroraGameplayTags.Abilities_None);
	
	FGameplayTag AbilityStatus;
	const FGameplayAbilitySpec* AbilitySpec = GetAuroraASC()->GetSpecFromAbilityTag(InAbilityTag);
	const bool bSpecValid = AbilitySpec != nullptr;
	
	if (!bTagValid || bTagNone || !bSpecValid)
	{
		AbilityStatus = AuroraGameplayTags.Abilities_Status_Locked;
	}
	else
	{
		AbilityStatus = GetAuroraASC()->GetAbilityStatusTagFromSpec(*AbilitySpec);
	}

	SelectedAbility.AbilityTag = InAbilityTag;
	SelectedAbility.StatusTag = AbilityStatus;
	
	bool bEnableSpendPoints = false;
	bool bEnableEquip = false;
	FString Description;
	FString NextLevelDescription;
				
	GetAuroraASC()->GetDescriptionsByAbilityTag(InAbilityTag, Description, NextLevelDescription);
	ShouldEnableButtons(AbilityStatus, SpellPoints, bEnableSpendPoints, bEnableEquip);
	OnSpellGlobeSelected.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
}
void USpellMenuWidgetController::SpendPointButtonPressed()
{
	if (!GetAuroraASC()) return;
	
	GetAuroraASC()->ServerSpendSpellPoint(SelectedAbility.AbilityTag);
}
void USpellMenuWidgetController::GlobeDeselect()
{
	SelectedAbility.AbilityTag = FAuroraGameplayTags::Get().Abilities_None;
	SelectedAbility.StatusTag = FAuroraGameplayTags::Get().Abilities_Status_Locked;

	OnSpellGlobeSelected.Broadcast(false, false, FString(), FString());
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints,
                                                     bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton)
{
	const FAuroraGameplayTags AuroraGameplayTags = FAuroraGameplayTags::Get();

	// SpellPoints and Equip buttons are disabled by default
	bShouldEnableSpellPointsButton = false;
	bShouldEnableEquipButton = false;
	
	// If Spell Equipped...
	if (AbilityStatus.MatchesTagExact(AuroraGameplayTags.Abilities_Status_Equipped))
	{
		//bShouldEnableEquipButton = true; // Equip button enable 
		if (SpellPoints > 0) bShouldEnableSpellPointsButton = true; // SpellPoints button enable if Spell points > 0 
	}

	// If Spell Eligible...
	else if (AbilityStatus.MatchesTagExact(AuroraGameplayTags.Abilities_Status_Eligible))
	{
		if (SpellPoints > 0) bShouldEnableSpellPointsButton = true;
	}

	// If Spell Unlocked...
	else if (AbilityStatus.MatchesTagExact(AuroraGameplayTags.Abilities_Status_Unlocked))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0) bShouldEnableSpellPointsButton = true;
	}

	// If Spell Locked...
	else if (AbilityStatus.MatchesTagExact(AuroraGameplayTags.Abilities_Status_Locked))
	{
		// Do nothing - both buttons are stays disabled.
	}
}
