#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Saves/LoadScreenSaveGame.h"
#include "AuroraGameModeBase.generated.h"

class ULootTiers;
class UGameplayStatics;
class ULoadScreenSaveGame;
class USaveGame;
class UMVVM_LoadSlot;
class UAuroraColors;
class UAbilityInfo;
class UCharacterClassInfo;

UCLASS()
class AURORA_API AAuroraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	
	virtual void BeginPlay() override;

public:

	static	void DeleteSlot(const FText& SlotName, int32 SlotIndex);
	
	void SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex);
	void TravelToMap(UMVVM_LoadSlot* Slot);
	void SaveInGameProgressData(ULoadScreenSaveGame* SaveObject);
	
	void SaveWorldState(UWorld* World, const FString& DestinationMapAssetName = FString("")) const;
	void LoadWorldState(UWorld* World) const;
	
	ULoadScreenSaveGame* RetrieveInGameSaveData();
	ULoadScreenSaveGame* GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const;

	void PlayerDied(ACharacter* DeadCharacter);
	
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	TObjectPtr<ULootTiers> LootTiers;

	UPROPERTY(EditDefaultsOnly, Category = "Aurora Colors", DisplayName = "Aurora Colors")
	TObjectPtr<UAuroraColors> AuroraColorsMap;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;

	UPROPERTY(EditDefaultsOnly)
	FString DefaultMapName;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DefaultMap;

	UPROPERTY(EditDefaultsOnly)
	FName DefaultPlayerStartTag; 

	UPROPERTY(EditDefaultsOnly)
	TMap<FString, TSoftObjectPtr<UWorld>> Maps;

	FString GetMapNameFromMapAssetName(const FString& MapAssetName) const;
};
