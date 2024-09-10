#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuroraAbilitySystemComponent.h"
#include "AbilitySystem/AuroraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
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
	AuroraASC = !AuroraASC ? Cast<UAuroraAbilitySystemComponent>(AbilitySystemComponent) : AuroraASC;
	
	// On Health changed
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuroraAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
	
	// On Max Health changed
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuroraAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
	
	// On Mana changed 
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuroraAttributeSet->GetManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnManaChanged.Broadcast(Data.NewValue);
			}
		);

	// On Max Mana changed
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuroraAttributeSet->GetMaxManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxManaChanged.Broadcast(Data.NewValue);
			}
		);


	if (AuroraASC)
	{
		
		if (AuroraASC->bStartupAbilitiesGiven)
		{
			OnInitializeStartupAbilities(AuroraASC);
		}
		else
		{
			// On ability given
			AuroraASC->OnAbilitiesGiven.AddUObject(this, &ThisClass::OnInitializeStartupAbilities);
		}

		// On Message tag received 
		AuroraASC->OnEffectAssetTagsApplied.AddLambda(
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

// TODO: Think about to bind function pointer to a BroadcastDelegate instead lambda
void UOverlayWidgetController::OnInitializeStartupAbilities(UAuroraAbilitySystemComponent* AuroraAbilitySystemComponent)
{
	if (!AuroraAbilitySystemComponent->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;

	BroadcastDelegate.BindLambda(
		[this, AuroraAbilitySystemComponent](const FGameplayAbilitySpec& AbilitySpec)
		{
			// Try to get ability info if it has an "Abilities" tag.
			FAuroraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AuroraAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));

			// 
			Info.InputTag = AuroraAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
			
			AbilityInfoDelegate.Broadcast(Info);
		});

	AuroraAbilitySystemComponent->ForEachAbility(BroadcastDelegate);
}

void UOverlayWidgetController::BroadcastAbilityInfo(const FGameplayAbilitySpec& AbilitySpec)
{
	AuroraASC = !AuroraASC ? Cast<UAuroraAbilitySystemComponent>(AbilitySystemComponent) : AuroraASC;
	
	// Try to get ability info if it has an "Abilities" tag.
	FAuroraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AuroraASC->GetAbilityTagFromSpec(AbilitySpec));

	// 
	Info.InputTag = AuroraASC->GetInputTagFromSpec(AbilitySpec);
			
	AbilityInfoDelegate.Broadcast(Info);
}

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{	
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}
