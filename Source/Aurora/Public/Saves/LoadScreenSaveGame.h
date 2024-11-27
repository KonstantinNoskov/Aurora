#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LoadScreenSaveGame.generated.h"

/*
 *  The Order is important. Enum index corresponds with a widget switcher slot index.
 */
UENUM(BlueprintType)
enum ESaveSlotStatus
{
	Vacant,
	EnterName,
	Taken
};

UCLASS()
class AURORA_API ULoadScreenSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FString SlotName = FString();

	UPROPERTY()
	int32 SlotIndex = 0;

	UPROPERTY()
	FString PlayerName = FString("");
	
	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = Vacant;

	UPROPERTY()
	bool bFirstTimeLoadIn = true;

	UPROPERTY()
	FString MapName = FString("");

	UPROPERTY()
	FName PlayerStartTag;

#pragma region Data to Save

#pragma region PlayerState

	UPROPERTY()
	int32 PlayerLevel = 0;

	UPROPERTY()
	int32 XP = 0;
	
	UPROPERTY()
	int32 SpellPoints = 0;

	UPROPERTY()
	int32 AttributePoints = 0;
	
#pragma endregion
#pragma region Attributes

	UPROPERTY()
	float Strength = 0;

	UPROPERTY()
	float Intelligence = 0;

	UPROPERTY()
	float Resilience = 0;

	UPROPERTY()
	float Vigor = 0;

#pragma endregion

#pragma endregion
	
};
