#include "UI/WidgetController/AuroraWidgetController.h"

void UAuroraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController		= WCParams.PlayerController;
	PlayerState				= WCParams.PlayerState;
	AbilitySystemComponent	= WCParams.AbilitySystemComponent;
	AttributeSet			= WCParams.AttributeSet;
}
