#pragma once

#include "CoreMinimal.h"
#include "AuroraCharacterBase.h"
#include "Interfaces/Interaction/TargetInterface.h"
#include "UI/WidgetController/AuroraWidgetController.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AuroraEnemy.generated.h"

class AAuroraAIController;
class UBehaviorTree;
class UWidgetComponent;

UCLASS()
class AURORA_API AAuroraEnemy : public AAuroraCharacterBase, public ITargetInterface
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

#pragma region CHARACTER CLASS DEFAULTS

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;
	
#pragma endregion

	void BindingHealthBarCallbacks();
	

public:

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetHitReacting(bool NewHitReacting) { bHitReacting = NewHitReacting;}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 250.f;
	
#pragma region TARGET INTERFACE

protected:
	
	virtual void HighLightActor() override;
	virtual void UnHighLightActor() override;

public:
	virtual int32 GetPlayerLevel() override;
	

	UPROPERTY(BlueprintReadOnly)
	bool bHighlighted = false;

#pragma endregion
#pragma region COMBAT INTERFACE

	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;
	
	virtual void Die() override;
	
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
	
};
