﻿#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "GameModes/AuroraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlots.Add(0, LoadSlot_0);
	LoadSlot_0->SetLoadSlotName(FString("LoadSlot_0"));
	
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlots.Add(1, LoadSlot_1);
	LoadSlot_1->SetLoadSlotName(FString("LoadSlot_1"));
	
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlots.Add(2, LoadSlot_2);
	LoadSlot_2->SetLoadSlotName(FString("LoadSlot_2"));
	
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex(int32 Index) const
{
	return LoadSlots.FindChecked(Index);
}

void UMVVM_LoadScreen::NewSlotButtonPressed(int32 Slot/*, const FString& EnteredName*/)
{
	AAuroraGameModeBase* AuroraGameMode = Cast<AAuroraGameModeBase>(UGameplayStatics::GetGameMode(this));

	//LoadSlots[Slot]->SetPlayerName(EnteredName);
	LoadSlots[Slot]->SlotStatus = Taken;
	AuroraGameMode->SaveSlotData(LoadSlots[Slot], Slot);
	
	LoadSlots[Slot]->InitializeSlot();
}

void UMVVM_LoadScreen::NewGameButtonPressed(int32 Slot)
{
	LoadSlots[Slot]->SetWidgetSwitcherIndex.Broadcast(1);
	
}

void UMVVM_LoadScreen::SelectSlotButtonPressed(int32 Slot)
{
}

void UMVVM_LoadScreen::LoadData()
{
	AAuroraGameModeBase* AuroraGameModeBase = Cast<AAuroraGameModeBase>(UGameplayStatics::GetGameMode(this));
	
	for (auto LoadSlot : LoadSlots)
	{
		ULoadScreenSaveGame* SaveObject = AuroraGameModeBase->GetSaveSlotData(LoadSlot.Value->GetLoadSlotName(), LoadSlot.Key);

		const FString PlayerName = SaveObject->PlayerName;
		TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = SaveObject->SaveSlotStatus;

		LoadSlot.Value->SlotStatus;
		LoadSlot.Value->SetPlayerName(FText::FromString(PlayerName));
		LoadSlot.Value->SlotStatus = SaveSlotStatus;
		LoadSlot.Value->InitializeSlot();
	}
}
