#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/Interaction/TargetInterface.h"
#include "AuroraPlayerController.generated.h"


class UAuroraAbilitySystemComponent;
// Input
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class UAuroraInputConfig;

UCLASS()
class AURORA_API AAuroraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuroraPlayerController();
	
	virtual void PlayerTick(float DeltaTime) override;


protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

#pragma region INPUT
	
private:	

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuroraMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	
	void Move(const FInputActionValue& InputActionValue);

#pragma endregion


#pragma region ABILITIES

private:
	
	UPROPERTY()
	TObjectPtr<UAuroraAbilitySystemComponent> AuroraAbilitySystemComponent; 
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuroraInputConfig> InputConfig;

	UFUNCTION()
	UAuroraAbilitySystemComponent* GetASC();
	
	UFUNCTION()
	void AbilityInputTagPressed(FGameplayTag InputTag);

	UFUNCTION()
	void AbilityInputTagReleased(FGameplayTag InputTag);

	UFUNCTION()
	void AbilityInputTagHeld(FGameplayTag InputTag);
	
#pragma endregion
	

	void CursorTrace();
	
	TScriptInterface<ITargetInterface> LastActor;
	TScriptInterface<ITargetInterface> ThisActor;

	
};
