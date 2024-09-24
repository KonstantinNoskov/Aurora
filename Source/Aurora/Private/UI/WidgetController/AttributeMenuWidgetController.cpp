#include "UI/WidgetController/AttributeMenuWidgetController.h"

//#include "AuroraGameplayTags.h"
#include "AbilitySystem/AuroraAbilitySystemComponent.h"
#include "AbilitySystem/AuroraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/AuroraPlayerState.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	// Getting actor's attribute set info
	
	// Looping through all attributes the actor has. 
	for (auto& Pair : GetAuroraAS()->TagsToAttributes)
	{
		// ... then bind callbacks to each one of this attribute's OnChangeDelegate
		GetAuroraASC()->GetGameplayAttributeValueChangeDelegate(
			Pair.Value()).AddLambda
			(
				[this, Pair](const FOnAttributeChangeData& Data)
				{
					// This function give a command to attribute menu widget to update values of all attribute's
					// have been changed.
					// Pair.Key - Attribute Tag. Pair.Value - Attribute itself
					BroadcastAttributeInfo(Pair.Key, Pair.Value());
				}
			);
	}

	// On Attribute Points changed
	GetAuroraPS()->OnAttributePointsChanged.AddUObject(this, &UAttributeMenuWidgetController::OnAttributePointsUpdate);

	// On Spell Points changed
}

/* BroadcastAttributeInfo
 *
 *	
 */
void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
	const FGameplayAttribute& Attribute) const
{
	// Fill Info struct by DataAsset we defined in blueprint
	FAuroraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);

	// set it's value equal to actor's attribute set
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);

	// Broadcast new values
	AttributeInfoDelegate.Broadcast(Info);
}

void UAttributeMenuWidgetController::OnAttributePointsUpdate(int32 AttributePoints) const
{
	OnAttributePointsUpdated.Broadcast(AttributePoints);
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	GetAuroraASC()->UpgradeAttribute(AttributeTag);
}

/* BroadcastInitialValues
 *	
 *	Updating actor's attributes such as Strength, AbilityPoints, etc at the start of the game
 */

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	check(AttributeInfo)
	
	// Initial Attribute stats (strength, intelligence, etc.)
	for (auto& Pair : GetAuroraAS()->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
	
	// Initial Attribute Points
	GetAuroraPS()->OnAttributePointsChanged.Broadcast(GetAuroraPS()->GetAttributePoints());
}


