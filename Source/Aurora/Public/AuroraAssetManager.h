#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "AuroraAssetManager.generated.h"


UCLASS()
class AURORA_API UAuroraAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	
	static UAuroraAssetManager& Get();

protected:

	virtual void StartInitialLoading() override;
};
