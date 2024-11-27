#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AuroraGameInstance.generated.h"

UCLASS()
class AURORA_API UAuroraGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FName PlayerStartTag = FName();

	UPROPERTY()
	FString LoadSlotName = FString();

	int32 LoadSlotIndex = 0;
};
