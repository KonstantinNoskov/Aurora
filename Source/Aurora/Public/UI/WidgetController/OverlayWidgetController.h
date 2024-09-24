#pragma once

#include "CoreMinimal.h"
#include "AuroraWidgetController.h"
#include "OverlayWidgetController.generated.h"

struct FAuroraAbilityInfo;
class UAuroraAbilitySystemComponent;
class UAbilityInfo;
class UAuroraUserWidget;

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UAuroraUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelChangedSignature, int32, NewStatValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);



UCLASS(BlueprintType, Blueprintable)
class AURORA_API UOverlayWidgetController : public UAuroraWidgetController
{
	GENERATED_BODY()

public:

	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

protected:

	UPROPERTY()
	UAuroraAbilitySystemComponent* AuroraASC;

#pragma region OVERLAY MESSAGES 
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);

	UPROPERTY(BlueprintAssignable, Category = "GAS | Messages")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;

#pragma endregion

#pragma region Attributes

protected:
	
	FOnGameplayAttributeValueChange OnGameplayAttributeValueChange;

public:
	
	UPROPERTY(BlueprintAssignable, Category = "GAS | Attributes")
	FOnAttributeChangedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS | Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS | Attributes")
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS | Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

#pragma endregion

#pragma region XP & Level UP

	UPROPERTY(BlueprintAssignable, Category = "GAS | XP")
	FOnLevelChangedSignature OnLevelUpdated;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS | XP")
	FOnAttributeChangedSignature OnXPUpdated;

	UFUNCTION()
	void OnXPUpdate(int32 NewXP);

	/*UFUNCTION()
	void OnXPUpdate(int32 NewXP) const;*/

#pragma endregion


};











