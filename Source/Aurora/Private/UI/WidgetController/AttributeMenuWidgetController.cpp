#include "UI/WidgetController/AttributeMenuWidgetController.h"

//#include "AuroraGameplayTags.h"
#include "AbilitySystem/AuroraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/AuroraPlayerState.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	// Getting actor's attribute set info
	const UAuroraAttributeSet* AS = CastChecked<UAuroraAttributeSet>(AttributeSet);
	AuroraPlayerState = !AuroraPlayerState ? CastChecked<AAuroraPlayerState>(PlayerState) : AuroraPlayerState;

	// Looping through all attributes the actor has. 
	for (auto& Pair : AS->TagsToAttributes)
	{
		// ... then bind callbacks to each one of this attribute's OnChangeDelegate
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
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
	AuroraPlayerState->OnAttributePointsChanged.AddUObject(this, &UAttributeMenuWidgetController::OnAttributePointsUpdate);

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


/* BroadcastInitialValues
 *	
 *	Updating actor's attributes at the start of the game
 */

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	const UAuroraAttributeSet* AS = CastChecked<UAuroraAttributeSet>(AttributeSet);
	AuroraPlayerState = !AuroraPlayerState ? CastChecked<AAuroraPlayerState>(PlayerState) : AuroraPlayerState;
	check(AttributeInfo)
	
	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
	
	AuroraPlayerState->OnAttributePointsChanged.Broadcast(AuroraPlayerState->GetAttributePoints());
}


