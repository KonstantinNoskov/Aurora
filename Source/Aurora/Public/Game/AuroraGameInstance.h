#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AuroraGameInstance.generated.h"

UCLASS()
class AURORA_API UAuroraGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	FName PlayerStartTag = FName();

	UPROPERTY(BlueprintReadOnly)
	FString LoadSlotName = FString();

	UPROPERTY(BlueprintReadOnly)
	int32 LoadSlotIndex = 0;
};
