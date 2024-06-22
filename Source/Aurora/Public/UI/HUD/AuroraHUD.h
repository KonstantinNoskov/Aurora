#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuroraHUD.generated.h"

class UAuroraUserWidget;

UCLASS()
class AURORA_API AAuroraHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:

	UPROPERTY()
	TObjectPtr<UAuroraUserWidget> OverlayWidget;

private:
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuroraUserWidget> OverlayWidgetClass;
};
