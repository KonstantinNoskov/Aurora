#pragma once

#include "CoreMinimal.h"
#include "AuroraCharacterBase.h"
#include "Interfaces/Interaction/TargetInterface.h"
#include "UI/WidgetController/AuroraWidgetController.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AuroraEnemy.generated.h"

class UWidgetComponent;

UCLASS()
class AURORA_API AAuroraEnemy : public AAuroraCharacterBase, public ITargetInterface
{
	GENERATED_BODY()

public:
	AAuroraEnemy();

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
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;
	
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

	virtual void Die() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 5.f;
	
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
	
};
