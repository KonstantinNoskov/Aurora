#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuroraHUD.generated.h"

class USpellMenuWidgetController;
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



#pragma region OVERLAY WIDGET CONTROLLER

public:
	
	UFUNCTION()
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WidgetControllerParams);
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	
private:
	
	UPROPERTY(EditAnywhere, Category = "Overlay")
	TSubclassOf<UAuroraUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UAuroraUserWidget> OverlayWidget;
	
	UPROPERTY(EditAnywhere, Category = "Overlay")
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

#pragma endregion	

#pragma region ATTRIBUTE MENU WIDGET CONTROLLER

public:
	
	UFUNCTION()
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WidgetControllerParams);

	void InitAttributeMenu(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

private:
	
	UPROPERTY(EditDefaultsOnly, Category = "Attribute Menu")
	TSubclassOf<UAuroraUserWidget> AttributeMenuWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UAuroraUserWidget> AttributeMenuWidget;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attribute Menu")
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

#pragma endregion

#pragma region Spell Menu Widget Controller

public:

	UFUNCTION()
	USpellMenuWidgetController* GetSpellMenuWidgetController(const FWidgetControllerParams& WidgetControllerParams);

	void InitSpellMenu(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	

private:

	UPROPERTY(EditDefaultsOnly, Category = "Spell Menu")
	TSubclassOf<UAuroraUserWidget> SpellMenuWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UAuroraUserWidget> SpellMenuWidget;
	
	UPROPERTY(EditDefaultsOnly, Category = "Spell Menu")
	TSubclassOf<USpellMenuWidgetController> SpellMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<USpellMenuWidgetController> SpellMenuWidgetController;
	
#pragma endregion

	

};
