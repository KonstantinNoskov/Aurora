#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuroraUserWidget.generated.h"


UCLASS()
class AURORA_API UAuroraUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

protected:
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnWidgetControllerSet();
	
};
