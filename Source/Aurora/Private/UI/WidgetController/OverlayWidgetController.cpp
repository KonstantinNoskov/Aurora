#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuroraAbilitySystemComponent.h"
#include "AbilitySystem/AuroraAttributeSet.h"
#include "Debug/DebugMacros.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UAuroraAttributeSet* AuroraAttributeSet = CastChecked<UAuroraAttributeSet>(AttributeSet);
	
	OnHealthChanged.Broadcast(AuroraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuroraAttributeSet->GetMaxHealth());

	OnManaChanged.Broadcast(AuroraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuroraAttributeSet->GetMaxMana());
}

// Bind callbacks to attribute changed events 

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UAuroraAttributeSet* AuroraAttributeSet = CastChecked<UAuroraAttributeSet>(AttributeSet);

	// Health 
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuroraAttributeSet->GetHealthAttribute()).AddLambda
		(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
	
	// Max Health
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuroraAttributeSet->GetMaxHealthAttribute()).AddLambda
		(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
	
	// Mana 
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuroraAttributeSet->GetManaAttribute()).AddLambda
		(
			[this](const FOnAttributeChangeData& Data)
			{
				OnManaChanged.Broadcast(Data.NewValue);
			}
		);

	// Max Mana
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuroraAttributeSet->GetMaxManaAttribute()).AddLambda
		(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxManaChanged.Broadcast(Data.NewValue);
			}
		);

	// On Ability tags applied
	Cast<UAuroraAbilitySystemComponent>(AbilitySystemComponent)->OnEffectAssetTagsApplied.AddLambda
	(
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

