#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuroraHUD.generated.h"

class UAttributeMenuWidgetController;
class UAbilitySystemComponent;
class UAttributeSet;

class UOverlayWidgetController;
class UAuroraUserWidget;

struct FWidgetControllerParams;

UCLASS()
class AURORA_API AAuroraHUD : public AHUD
{
	GENERATED_BODY()

private:

#pragma region OVERLAY WIDGET CONTROLLER
	
	UPROPERTY(EditAnywhere, Category = "Overlay")
	TSubclassOf<UAuroraUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UAuroraUserWidget> OverlayWidget;
	
	UPROPERTY(EditAnywhere, Category = "Overlay")
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

public:
	
	UFUNCTION()
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WidgetControllerParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	
#pragma endregion	

#pragma region ATTRIBUTE MENU WIDGET CONTROLLER
	
	UPROPERTY(EditAnywhere, Category = "Attribute Menu")
	TSubclassOf<UAuroraUserWidget> AttributeMenuWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UAuroraUserWidget> AttributeMenuWidget;
	
	UPROPERTY(EditAnywhere, Category = "Attribute Menu")
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

public:
	
	UFUNCTION()
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WidgetControllerParams);

	void InitAttributeMenu(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	
#pragma endregion



	

};
