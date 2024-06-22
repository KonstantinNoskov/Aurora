#include "UI/Widgets/AuroraUserWidget.h"

void UAuroraUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	
	OnWidgetControllerSet();

	
}
