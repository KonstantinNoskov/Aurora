#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuroraHUD.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

class UOverlayWidgetController;
class UAuroraUserWidget;

struct FWidgetControllerParams;

UCLASS()
class AURORA_API AAuroraHUD : public AHUD
{
	GENERATED_BODY()

public:
	
	UPROPERTY()
	TObjectPtr<UAuroraUserWidget> OverlayWidget;

private:

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuroraUserWidget> OverlayWidgetClass;

public:

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	
	UFUNCTION()
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WidgetControllerParams);
};
