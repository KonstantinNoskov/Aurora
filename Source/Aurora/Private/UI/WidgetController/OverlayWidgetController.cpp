#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuroraAbilitySystemComponent.h"
#include "AbilitySystem/AuroraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/AuroraPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UAuroraAttributeSet* AuroraAttributeSet = CastChecked<UAuroraAttributeSet>(AttributeSet);
	
	OnHealthChanged.Broadcast(AuroraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuroraAttributeSet->GetMaxHealth());

	OnManaChanged.Broadcast(AuroraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuroraAttributeSet->GetMaxMana());
}

// Bind callbacks to different events on Ability System Component, Attribute Set, Player state, etc.
// so the widget controller can give commands to the widgets
void UOverlayWidgetController::BindCallbacksToDependencies()
{
	AuroraASC = !AuroraASC ? Cast<UAuroraAbilitySystemComponent>(AbilitySystemComponent) : AuroraASC;
	const UAuroraAttributeSet* AuroraAttributeSet = CastChecked<UAuroraAttributeSet>(AttributeSet);
	AAuroraPlayerState* AuroraPlayerState = CastChecked<AAuroraPlayerState>(PlayerState);
	
	// On Health changed
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuroraAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			});
	
	// On Max Health changed
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuroraAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			});
	
	// On Mana changed 
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuroraAttributeSet->GetManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnManaChanged.Broadcast(Data.NewValue);
			});

	// On Max Mana changed
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuroraAttributeSet->GetMaxManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxManaChanged.Broadcast(Data.NewValue);
			});
	
	// On XP changed
	//AuroraPlayerState->OnXPChanged.AddUObject(this, &UOverlayWidgetController::OnXPUpdate);

	// On XP changed TEST
	AuroraPlayerState->OnXPChanged.AddUObject(this, &UOverlayWidgetController::OnXPUpdate);

	// On Level changed
	AuroraPlayerState->OnLevelChanged.AddLambda(
	[this](int32 NewLevel)
			{
				OnLevelUpdated.Broadcast(NewLevel);
			});
	
	// On Startup Abilities Initialized
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
void UOverlayWidgetController::OnInitializeStartupAbilities(UAuroraAbilitySystemComponent* AuroraAbilitySystemComponent)
{
	if (!AuroraAbilitySystemComponent->bStartupAbilitiesGiven) return;

	// Create a delegate 
	FForEachAbility BroadcastDelegate;

	// Bind AbilityInfo setup to it
	BroadcastDelegate.BindUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);

	// Pass in outcome delegate to the ASC ForEachAbility-function
	// so it can execute the delegate for each activatable ability
	AuroraAbilitySystemComponent->ForEachAbility(BroadcastDelegate);
}
void UOverlayWidgetController::BroadcastAbilityInfo(const FGameplayAbilitySpec& AbilitySpec)
{
	AuroraASC = !AuroraASC ? Cast<UAuroraAbilitySystemComponent>(AbilitySystemComponent) : AuroraASC;
	
	// Try to get ability info if it has an "Abilities" tag.
	FAuroraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AuroraASC->GetAbilityTagFromSpec(AbilitySpec));

	// Set AbilityInfo InputTag if ability have it
	Info.InputTag = AuroraASC->GetInputTagFromSpec(AbilitySpec);

	// Broadcast outcome AbilityInfo to the widgets
	AbilityInfoDelegate.Broadcast(Info);
}

void UOverlayWidgetController::OnXPUpdate(int32 NewXP) const
{
	const AAuroraPlayerState* AuroraPlayerState = CastChecked<AAuroraPlayerState>(PlayerState);
	const ULevelUpInfo* LevelUpInfo = AuroraPlayerState->LevelUpInfo;

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



template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{	
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}
