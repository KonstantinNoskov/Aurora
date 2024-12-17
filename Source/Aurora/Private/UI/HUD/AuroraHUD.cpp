#include "UI/HUD/AuroraHUD.h"

#include "Blueprint/UserWidget.h"
#include "Debug/DebugMacros.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "UI/Widgets/AuroraUserWidget.h"

#pragma region OVERLAY WIDGET CONTROLLER

void AAuroraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass,				TEXT("Overlay Widget Class uninitialized, fill out BP_AuroraHUD"));
	checkf(OverlayWidgetControllerClass,	TEXT("Overlay Widget Controller Class uninitialized, fill out BP_AuroraHUD"));
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuroraUserWidget>(Widget);
	
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
	Widget->AddToViewport();
	
}

UOverlayWidgetController* AAuroraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (!OverlayWidgetController)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}


	return OverlayWidgetController; 
}

#pragma endregion

#pragma region ATTRIBUTE MENU WIDGET CONTROLLER

void AAuroraHUD::InitAttributeMenu(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC,
	UAttributeSet* AS)
{
	checkf(AttributeMenuWidgetClass,				TEXT("Attribute Menu widget class uninitialized, fill out BP_AuroraHUD"));
	checkf(AttributeMenuWidgetControllerClass,		TEXT("Attribute Menu widget controller class uninitialized, fill out BP_AuroraHUD"));
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), AttributeMenuWidgetClass);
	AttributeMenuWidget = Cast<UAuroraUserWidget>(Widget);
	
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UAttributeMenuWidgetController* WidgetController = GetAttributeMenuWidgetController(WidgetControllerParams);

	AttributeMenuWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
}


UAttributeMenuWidgetController* AAuroraHUD::GetAttributeMenuWidgetController(
	const FWidgetControllerParams& WCParams)
{
	if (!AttributeMenuWidgetController)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}

	return AttributeMenuWidgetController; 
}

#pragma endregion
#pragma region Spell Menu Widget Controller

USpellMenuWidgetController* AAuroraHUD::GetSpellMenuWidgetController(
	const FWidgetControllerParams& WidgetControllerParams)
{
	if (!SpellMenuWidgetController)
	{
		SpellMenuWidgetController = NewObject<USpellMenuWidgetController>(this, SpellMenuWidgetControllerClass);
		SpellMenuWidgetController->SetWidgetControllerParams(WidgetControllerParams);
		SpellMenuWidgetController->BindCallbacksToDependencies();
	}
	
	return SpellMenuWidgetController;
}

void AAuroraHUD::InitSpellMenu(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(SpellMenuWidgetClass,				TEXT("Spell Menu widget class uninitialized, fill out BP_AuroraHUD"));
	checkf(SpellMenuWidgetControllerClass,		TEXT("Spell Menu widget controller class uninitialized, fill out BP_AuroraHUD"));
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), SpellMenuWidgetClass);
	SpellMenuWidget = Cast<UAuroraUserWidget>(Widget);
	
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	USpellMenuWidgetController* WidgetController = GetSpellMenuWidgetController(WidgetControllerParams);

	SpellMenuWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
}


#pragma endregion


