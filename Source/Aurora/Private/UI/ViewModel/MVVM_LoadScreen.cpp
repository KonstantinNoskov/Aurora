#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "Game/AuroraGameInstance.h"
#include "GameModes/AuroraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlots.Add(0, LoadSlot_0);
	LoadSlot_0->SetLoadSlotName(FString("LoadSlot_0"));
	LoadSlot_0->SetSlotIndex(0);
	
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlots.Add(1, LoadSlot_1);
	LoadSlot_1->SetLoadSlotName(FString("LoadSlot_1"));
	LoadSlot_1->SetSlotIndex(1);
	
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlots.Add(2, LoadSlot_2);
	LoadSlot_2->SetLoadSlotName(FString("LoadSlot_2"));
	LoadSlot_2->SetSlotIndex(2);
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex(int32 Index) const
{
	return LoadSlots.FindChecked(Index);
}

void UMVVM_LoadScreen::NewSlotButtonPressed(int32 Slot/*, const FText& EnteredName*/)
{
	AAuroraGameModeBase* AuroraGameMode = Cast<AAuroraGameModeBase>(UGameplayStatics::GetGameMode(this));

	//LoadSlots[Slot]->SetPlayerName(EnteredName);
	LoadSlots[Slot]->SlotStatus = Taken;
	LoadSlots[Slot]->SetMapName(AuroraGameMode->DefaultMapName);
	LoadSlots[Slot]->PlayerStartTag = AuroraGameMode->DefaultPlayerStartTag;
	
	AuroraGameMode->SaveSlotData(LoadSlots[Slot], Slot);
	LoadSlots[Slot]->InitializeSlot();

	UAuroraGameInstance* AuroraGameInstance = Cast<UAuroraGameInstance>(AuroraGameMode->GetGameInstance());
	AuroraGameInstance->LoadSlotName = LoadSlots[Slot]->GetLoadSlotName();
	AuroraGameInstance->LoadSlotIndex = LoadSlots[Slot]->GetSlotIndex();
	AuroraGameInstance->PlayerStartTag = AuroraGameMode->DefaultPlayerStartTag;
}

void UMVVM_LoadScreen::NewGameButtonPressed(int32 Slot)
{
	LoadSlots[Slot]->SetWidgetSwitcherIndex.Broadcast(1);
	SelectedSlot = nullptr;
	SetPlayButtonIsEnable(false);
	SetDeleteButtonIsEnable(false);
}

void UMVVM_LoadScreen::SelectSlotButtonPressed(int32 Slot)
{
	SlotSelected.Broadcast();
	for (auto LoadSlot : LoadSlots)
	{
		LoadSlot.Value->SetSelectButtonIsEnable(LoadSlot.Key != Slot);
		/*if (LoadSlot.Key == Slot)
		{
			LoadSlot.Value->EnableSelectSlotButton.Broadcast(false);
		}
		else
		{
			LoadSlot.Value->EnableSelectSlotButton.Broadcast(true);
		}*/
	}

	// Store selected slot
	SelectedSlot = LoadSlots[Slot];

	// Enable Play and Delete buttons in LoadScreen
	SetPlayButtonIsEnable(true);
	SetDeleteButtonIsEnable(true);
	
}

void UMVVM_LoadScreen::PlayButtonPressed()
{
	AAuroraGameModeBase* AuroraGameModeBase = Cast<AAuroraGameModeBase>(UGameplayStatics::GetGameMode(this));
	UAuroraGameInstance* AuroraGameInstance = Cast<UAuroraGameInstance>(AuroraGameModeBase->GetGameInstance());

	AuroraGameInstance->PlayerStartTag = SelectedSlot->PlayerStartTag;
	AuroraGameInstance->LoadSlotName = SelectedSlot->GetLoadSlotName();
	AuroraGameInstance->LoadSlotIndex = SelectedSlot->GetSlotIndex();
	
	if (IsValid(SelectedSlot))
	{
		AuroraGameModeBase->TravelToMap(SelectedSlot);	
	}
}

void UMVVM_LoadScreen::LoadData()
{
	AAuroraGameModeBase* AuroraGameModeBase = Cast<AAuroraGameModeBase>(UGameplayStatics::GetGameMode(this));
	
	for (auto LoadSlot : LoadSlots)
	{
		ULoadScreenSaveGame* SaveObject = AuroraGameModeBase->GetSaveSlotData(LoadSlot.Value->GetLoadSlotName(), LoadSlot.Key);

		const FString PlayerName = SaveObject->PlayerName;
		const FString MapName = SaveObject->MapName;
		TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = SaveObject->SaveSlotStatus;

		LoadSlot.Value->SlotStatus;
		LoadSlot.Value->SetPlayerName(FText::FromString(PlayerName));
		LoadSlot.Value->SlotStatus = SaveSlotStatus;
		LoadSlot.Value->InitializeSlot();
		LoadSlot.Value->SetMapName(MapName);
		LoadSlot.Value->PlayerStartTag = SaveObject->PlayerStartTag;
	}
}

void UMVVM_LoadScreen::DeleteButtonPressed()
{
	if (IsValid(SelectedSlot))
	{
		AAuroraGameModeBase::DeleteSlot(FText::FromString(SelectedSlot->GetLoadSlotName()), SelectedSlot->GetSlotIndex());
		SelectedSlot->SlotStatus = Vacant;
		SelectedSlot->InitializeSlot();
		SelectedSlot->SetSelectButtonIsEnable(true);
		SelectedSlot->SetPlayerName(FText());
		
		SetPlayButtonIsEnable(true);
		SetDeleteButtonIsEnable(true);
	}
	
}
