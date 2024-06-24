#include "UI/HUD/AuroraHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/Widgets/AuroraUserWidget.h"


void AAuroraHUD::InitOverlay(APlayerController* PC, APlayerState* PS,
	UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass,				TEXT("Overlay Widget Class uninitialized, fill out BP_AuroraHUD"));
	checkf(OverlayWidgetControllerClass,	TEXT("Overlay Widget Controller Class uninitialized, fill out BP_AuroraHUD"));
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuroraUserWidget>(Widget);
	
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);
	
	Widget->AddToViewport();	
}

UOverlayWidgetController* AAuroraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (!OverlayWidgetController)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);

		return OverlayWidgetController;
	}

	return OverlayWidgetController; 
}
