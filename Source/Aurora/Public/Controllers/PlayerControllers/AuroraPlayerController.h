﻿#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/Interaction/TargetInterface.h"
#include "AuroraPlayerController.generated.h"


class AMagicCircle;
class UNiagaraSystem;
class UDamageTextComponent;
class USplineComponent;
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

#pragma region DAMAGE
	
public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
	
	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);
	
#pragma endregion
#pragma region INPUT
	
private:	

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuroraMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;
	
	void Move(const FInputActionValue& InputActionValue);

	void ShiftPressed()		{ bShiftKeyDown = true; }
	void ShiftReleased()	{ bShiftKeyDown = false; }
	bool bShiftKeyDown = false;

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

public:
	
	UFUNCTION(BlueprintCallable)
	AMagicCircle* ShowMagicCircle(UMaterialInterface* DecalMaterial = nullptr, float InMagicCircleRadius = 0.f);

	UFUNCTION(BlueprintCallable)
	void HideMagicCircle();

	

	UFUNCTION()
	void UpdateMagicCircleLocation();

private:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> ClickNiagaraSystem;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMagicCircle> MagicCircleClass;

public:
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AMagicCircle> MagicCircle;
	
#pragma endregion
#pragma region MOVEMENT NAVIGATION

private:
	
	UPROPERTY()
	FVector CachedDestination = FVector::Zero();

	UPROPERTY()
	float FollowTime = 0.f;

	UPROPERTY()
	float ShortPressThreshold = .5f;

	UPROPERTY()
	bool bAutoRunning = false;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	UPROPERTY()
	bool bTargeting = false;
	
	void AutoRun();
	
#pragma endregion
	
	void CursorTrace();
	
	TScriptInterface<ITargetInterface> LastActor;
	TScriptInterface<ITargetInterface> ThisActor;

	FHitResult CursorHit;

	
};
