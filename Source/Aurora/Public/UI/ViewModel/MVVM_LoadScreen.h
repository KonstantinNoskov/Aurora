#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"

#include "MVVM_LoadScreen.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSlotSelected);

class UMVVM_LoadSlot;

UCLASS()
class AURORA_API UMVVM_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:

	void InitializeLoadSlots();

#pragma region MVVM Getters/Setters

	// Getters
	bool GetPlayButtonIsEnable() const 							{ return PlayButtonIsEnable; }
	bool GetDeleteButtonIsEnable() const 						{ return DeleteButtonIsEnable; }

	// Setters
	void SetPlayButtonIsEnable(bool bInPlayButtonIsEnable)		{ UE_MVVM_SET_PROPERTY_VALUE(PlayButtonIsEnable, bInPlayButtonIsEnable); }
	void SetDeleteButtonIsEnable(bool bInDeleteButtonIsEnable)	{ UE_MVVM_SET_PROPERTY_VALUE(DeleteButtonIsEnable, bInDeleteButtonIsEnable); }


#pragma endregion

	UFUNCTION(BlueprintPure)
	UMVVM_LoadSlot* GetLoadSlotViewModelByIndex(int32 Index) const;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadSlot> LoadSlotViewModelClass;

	UFUNCTION(BlueprintCallable)
	void NewSlotButtonPressed(int32 Slot/*, const FText& EnteredName*/);

	UFUNCTION(BlueprintCallable)
	void NewGameButtonPressed(int32 Slot);

	UFUNCTION(BlueprintCallable)
	void SelectSlotButtonPressed(int32 Slot);

	UFUNCTION(BlueprintCallable)
	void PlayButtonPressed();

	void LoadData();

public:

	UPROPERTY(BlueprintAssignable)
	FOnSlotSelected SlotSelected;
	
	UFUNCTION(BlueprintCallable)
	void DeleteButtonPressed();
	
private:

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> SelectedSlot;
	
	UPROPERTY()
	TMap<int32, UMVVM_LoadSlot*> LoadSlots;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_0;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_1;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	bool PlayButtonIsEnable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	bool DeleteButtonIsEnable = false;
};
