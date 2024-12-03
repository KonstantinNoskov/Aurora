#include "UI/WidgetController/OverlayWidgetController.h"

#include "AuroraGameplayTags.h"
#include "AbilitySystem/AuroraAbilitySystemComponent.h"
#include "AbilitySystem/AuroraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/AuroraPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	OnHealthChanged.Broadcast(GetAuroraAS()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetAuroraAS()->GetMaxHealth());

	OnManaChanged.Broadcast(GetAuroraAS()->GetMana());
	OnMaxManaChanged.Broadcast(GetAuroraAS()->GetMaxMana());
}

// Bind callbacks to different events on Ability System Component, Attribute Set, Player state, etc.
// so the widget controller can give commands to the widgets
void UOverlayWidgetController::BindCallbacksToDependencies()
{
	// On Health changed
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuroraAS()->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			});
	
	// On Max Health changed
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuroraAS()->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			});
	
	// On Mana changed 
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuroraAS()->GetManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnManaChanged.Broadcast(Data.NewValue);
			});

	// On Max Mana changed
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuroraAS()->GetMaxManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxManaChanged.Broadcast(Data.NewValue);
			});
	
	// On XP changed
	GetAuroraPS()->OnXPChanged.AddUObject(this, &UOverlayWidgetController::OnXPUpdate);
	
	// On Level changed
	GetAuroraPS()->OnLevelChanged.AddLambda(
	[this](int32 NewLevel, bool bLevelUp)
			{
				OnLevelUpdated.Broadcast(NewLevel, bLevelUp);
			});
	
	// On Startup Abilities Initialized
	if (GetAuroraASC())
	{
		// On Ability Equipped
		GetAuroraASC()->OnAbilityEquipped.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);
		
		if (GetAuroraASC()->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			// On ability given
			GetAuroraASC()->OnAbilitiesGiven.AddUObject(this, &ThisClass::BroadcastAbilityInfo);
		}

		// On Message tag received 
		GetAuroraASC()->OnEffectAssetTagsApplied.AddLambda(
			[this](const FGameplayTagContainer& AssetTags)
				{
					for (const FGameplayTag& Tag : AssetTags)
					{
						// For example, say that Tag = Message.HealthPotion
						// "Message.HealthPotion".MatchesTag("Message") will return True, "Message".MatchesTag("Message.HealthPotion") will return False
					
						FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
						if (Tag.MatchesTag(MessageTag))
						{
							const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
							MessageWidgetRowDelegate.Broadcast(*Row);
						}
					}
				}
		);
	}
}

void UOverlayWidgetController::OnXPUpdate(int32 NewXP) 
{
	const ULevelUpInfo* LevelUpInfo = GetAuroraPS()->LevelUpInfo;
	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

	if (Level <= MaxLevel && Level > 0)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PrevLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level-1]. LevelUpRequirement;
		const int32 DeltaLevelUpRequirement = LevelUpRequirement - PrevLevelUpRequirement;
		const int32 XPForThisLevel = NewXP - PrevLevelUpRequirement;
		
		const float XPBarPercentAsFloat = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelUpRequirement);

		OnXPUpdated.Broadcast(XPBarPercentAsFloat);
	}
}

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& AbilityStatusTag, const FGameplayTag& SlotTag, const FGameplayTag& PrevSlotTag) const
{
	
	const FAuroraGameplayTags GameplayTags = FAuroraGameplayTags::Get();
	
	FAuroraAbilityInfo LastSlotInfo;
	LastSlotInfo.AbilityStatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PrevSlotTag;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;

	// Broadcast empty info if PrevSlot is a valid slot. Only if equipping an already-equipped spell;
	//AbilityInfoDelegate.Broadcast(LastSlotInfo);

	FAuroraAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
	Info.AbilityStatusTag = AbilityStatusTag;
	Info.InputTag = SlotTag;
	
	AbilityInfoDelegate.Broadcast(Info);
	
}

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{	
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}
