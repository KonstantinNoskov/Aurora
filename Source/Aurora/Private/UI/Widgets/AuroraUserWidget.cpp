#include "UI/Widgets/AuroraUserWidget.h"

#include "UI/WidgetController/AuroraWidgetController.h"

void UAuroraUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	
	OnWidgetControllerSet();
}
