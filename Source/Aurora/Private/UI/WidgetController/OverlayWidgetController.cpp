#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuroraAttributeSet.h"

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
		AuroraAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);

	// Max Health
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuroraAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);

	// Health 
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuroraAttributeSet->GetManaAttribute()).AddUObject(this, &UOverlayWidgetController::ManaChanged);

	// Max Health
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuroraAttributeSet->GetMaxManaAttribute()).AddUObject(this, &UOverlayWidgetController::MaxManaChanged);
}


#pragma region HEALTH

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const 
{
	OnHealthChanged.Broadcast(Data.NewValue);
}
	
void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

#pragma endregion

#pragma region MANA

void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}

#pragma endregion
