﻿#pragma once

#include "CoreMinimal.h"
#include "AuroraCharacterBase.h"
#include "Interfaces/Interaction/TargetInterface.h"
#include "UI/WidgetController/AuroraWidgetController.h"
#include "Debug/DebugMacros.h"
#include "Interfaces/SaveInterface.h"
#include "Interfaces/Interaction/HighlightInterface.h"
#include "AuroraEnemy.generated.h"

class AAuroraAIController;
class UBehaviorTree;
class UWidgetComponent;

UCLASS()
class AURORA_API AAuroraEnemy :
public AAuroraCharacterBase,
public ITargetInterface,
public ISaveInterface,
public IHighlightInterface
{
	GENERATED_BODY()

public:
	AAuroraEnemy();

	virtual void PossessedBy(AController* NewController) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:

	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;

#pragma region CHARACTER CLASS DEFAULTS

public:

	UFUNCTION()
	void SetLevel(int32 InLevel) { Level = InLevel; }

#pragma endregion

	void BindingHealthBarCallbacks();

public:

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetHitReacting(bool NewHitReacting) { bHitReacting = NewHitReacting;}

protected:

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnLoot();
	
#pragma region Highlight Interface

protected:
	
	virtual void HighLightActor_Implementation() override;
	virtual void UnHighLightActor_Implementation() override;
	virtual void SetMoveToLocation_Implementation(FVector& OutDestination) override;

	virtual int32 GetPlayerLevel_Implementation() override;
	

	UPROPERTY(BlueprintReadOnly)
	bool bHighlighted = false;

#pragma endregion
#pragma region COMBAT INTERFACE

	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;
	
	virtual void Die(const FVector& InDeathImpulse) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 5.f;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

#pragma endregion
#pragma region UI

public:

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

#pragma endregion
#pragma region AI

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr<AAuroraAIController> AuroraAIController;

#pragma endregion

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

public:
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;
};

inline void AAuroraEnemy::SetMoveToLocation_Implementation(FVector& OutDestination)
{
	// DO NOT change OutDestination
}


	
