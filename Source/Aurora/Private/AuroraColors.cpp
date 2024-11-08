#include "AuroraColors.h"

#include "Aurora/AuroraLogChannels.h"

FColor UAuroraColors::FindColorByTag(const FGameplayTag& GameplayTag, bool bLogNotFound) const
{
	if (DamageTypeToColor.Contains(GameplayTag))
	{
		return DamageTypeToColor[GameplayTag];
	}
	
	if (bLogNotFound)
	{
		UE_LOG(LogAurora, Error, TEXT("[%hs]: Can't find Damage Type Color for AbilityTag [%s] on AuroraColor [%s]"), __FUNCTION__ , *GameplayTag.ToString(), *GetNameSafe(this));
	}

	return FColor();
}
