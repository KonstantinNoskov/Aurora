#include "Saves/LoadScreenSaveGame.h"

FSavedMap ULoadScreenSaveGame::GetSavedMapByMapName(const FString& InMapName)
{
	for (const FSavedMap Map : SavedMaps)
	{
		if (Map.MapAssetName == InMapName)
		{
			return Map;
		}
	}
	return FSavedMap();
}

bool ULoadScreenSaveGame::HasMap(const FString& InMapName)
{
	for (const FSavedMap Map : SavedMaps)
	{
		if (Map.MapAssetName == InMapName)
		{
			return true;
		}
	}
	return false;
}
