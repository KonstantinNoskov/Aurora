#pragma once

#include "CoreMinimal.h"
#include "AuroraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"


class AAuroraPlayerState;
class UAttributeInfo;
struct FAuroraAttributeInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAuroraAttributeInfo&, Info);

UCLASS(BlueprintType, Blueprintable)
class AURORA_API UAttributeMenuWidgetController : public UAuroraWidgetController
{
	GENERATED_BODY()

public:
	
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS | Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

protected:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;

	UPROPERTY(BlueprintAssignable, Category = "GAS | Attributes")
	FOnAttributeChangedSignature OnAttributePointsUpdated;

private:

	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;

	UFUNCTION()
	void OnAttributePointsUpdate(int32 AttributePoints) const;

	UPROPERTY()
	AAuroraPlayerState* AuroraPlayerState;
};
