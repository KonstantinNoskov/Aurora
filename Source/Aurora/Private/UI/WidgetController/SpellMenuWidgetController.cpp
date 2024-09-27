#include "UI/WidgetController/SpellMenuWidgetController.h"

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
		[this](const FGameplayTag& AbilityTag, const FGameplayTag& AbilityStatusTag)
		{
			if (AbilityInfo)
			{
				FAuroraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
				Info.AbilityStatusTag = AbilityStatusTag;

				AbilityInfoDelegate.Broadcast(Info);
			}
		});

	// Spell Point Change
	GetAuroraPS()->OnSpellPointsChanged.AddLambda([this](int32 SpellPoints)
	{
		SpellPointsUpdated.Broadcast(SpellPoints);
	});
}
