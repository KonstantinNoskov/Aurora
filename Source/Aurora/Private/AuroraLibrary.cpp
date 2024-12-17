#include "AuroraLibrary.h"

#include "GameModes/AuroraGameModeBase.h"
#include "Kismet/GameplayStatics.h"

UAuroraColors* UAuroraLibrary::GetAuroraColors(const UObject* WorldContextObject)
{
	AAuroraGameModeBase* AuroraGameMode = Cast<AAuroraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!AuroraGameMode) return nullptr;

	return AuroraGameMode->AuroraColorsMap;
}
