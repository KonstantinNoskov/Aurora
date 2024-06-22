#include "UI/HUD/AuroraHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widgets/AuroraUserWidget.h"

void AAuroraHUD::BeginPlay()
{
	Super::BeginPlay();

	TObjectPtr<UUserWidget> Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);

	Widget->AddToViewport();	
}
