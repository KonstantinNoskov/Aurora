#pragma once

#include "CoreMinimal.h"
#include "AuroraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

UCLASS()
class AURORA_API USpellMenuWidgetController : public UAuroraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
};
