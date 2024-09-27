#pragma once

#include "CoreMinimal.h"
#include "AuroraWidgetController.h"
#include "Player/AuroraPlayerState.h"
#include "SpellMenuWidgetController.generated.h"


UCLASS(BlueprintType, Blueprintable)
class AURORA_API USpellMenuWidgetController : public UAuroraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature SpellPointsUpdated;
};
