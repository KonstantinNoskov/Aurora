#include "UI/WidgetController/AuroraWidgetController.h"

#include "AbilitySystem/AuroraAbilitySystemComponent.h"
#include "AbilitySystem/AuroraAttributeSet.h"
#include "Controllers/PlayerControllers/AuroraPlayerController.h"
#include "Player/AuroraPlayerState.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "UI/WidgetController/OverlayWidgetController.h"

void UAuroraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController		= WCParams.PlayerController;
	PlayerState				= WCParams.PlayerState;
	AbilitySystemComponent	= WCParams.AbilitySystemComponent;
	AttributeSet			= WCParams.AttributeSet;
}

void UAuroraWidgetController::BroadcastInitialValues() { }

void UAuroraWidgetController::BindCallbacksToDependencies() { }

AAuroraPlayerController* UAuroraWidgetController::GetAuroraPC()
{
	if (!AuroraPlayerController)
	{
		AuroraPlayerController = Cast<AAuroraPlayerController>(PlayerController);
	}
	return AuroraPlayerController;
}
AAuroraPlayerState* UAuroraWidgetController::GetAuroraPS()
{
	if (!AuroraPlayerState)
	{
		AuroraPlayerState = Cast<AAuroraPlayerState>(PlayerState);
	}
	return AuroraPlayerState; 
}
UAuroraAbilitySystemComponent* UAuroraWidgetController::GetAuroraASC()
{
	if (!AuroraAbilitySystemComponent)
	{
		AuroraAbilitySystemComponent = Cast<UAuroraAbilitySystemComponent>(AbilitySystemComponent);
	}
	return AuroraAbilitySystemComponent; 
}
UAuroraAttributeSet* UAuroraWidgetController::GetAuroraAS()
{
	if (!AuroraAttributeSet)
	{
		AuroraAttributeSet = Cast<UAuroraAttributeSet>(AttributeSet);
	}
	return AuroraAttributeSet; 
}

void UAuroraWidgetController::BroadcastAbilityInfo()
{
	if (!GetAuroraASC()->bStartupAbilitiesGiven) return;

	// Create a delegate 
	FForEachAbility BroadcastDelegate;

	// Bind AbilityInfo setup to it
	BroadcastDelegate.BindUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);

	// Pass in outcome delegate to the ASC ForEachAbility-function
	// so it can execute the delegate for each activatable ability
	GetAuroraASC()->ForEachAbility(BroadcastDelegate);
}

void UAuroraWidgetController::BroadcastAbilityInfo(const FGameplayAbilitySpec& AbilitySpec)
{
	// Try to get ability info if it has an "Abilities" tag.
	FAuroraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(GetAuroraASC()->GetAbilityTagFromSpec(AbilitySpec));

	// Set AbilityInfo InputTag if ability have it
	Info.InputTag = GetAuroraASC()->GetInputTagFromSpec(AbilitySpec);

	// Broadcast outcome AbilityInfo to the widgets
	AbilityInfoDelegate.Broadcast(Info);
}
