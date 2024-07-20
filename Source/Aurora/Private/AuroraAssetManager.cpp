#include "AuroraAssetManager.h"

#include "AuroraGameplayTags.h"

UAuroraAssetManager& UAuroraAssetManager::Get()
{
	check(GEngine)

	UAuroraAssetManager* AuroraAssetManager = Cast<UAuroraAssetManager>(GEngine->AssetManager);
	return *AuroraAssetManager;
}

void UAuroraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FAuroraGameplayTags::InitializeNativeGameplayTags();
}
