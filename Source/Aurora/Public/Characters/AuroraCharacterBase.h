#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/Interaction/CombatInterface.h"
#include "AuroraCharacterBase.generated.h"

class UGameplayAbility;

// Ability System
class UAttributeSet;
class UGameplayEffect;


UCLASS(Abstract)
class AURORA_API AAuroraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()
	
public:
	
	AAuroraCharacterBase();

protected:

	virtual void BeginPlay() override;

protected:
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

#pragma region ABILITY SYSTEM

#pragma region ABILITIES
	
private:
	
	UPROPERTY(EditAnywhere, Category = "Abilitites")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

protected:

	UFUNCTION()
	void AddCharacterAbilities();

#pragma endregion
	
protected:
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	virtual void InitAbilityActorInfo();
	
	UFUNCTION()
	void InitializeDefaultAttributes() const;

public:

	UFUNCTION()
	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, const float Level) const;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	FORCEINLINE TObjectPtr<UAttributeSet> GetAttributeSet() const { return AttributeSet; }  

#pragma region ATTRIBUTES
	
#pragma region PRIMARY ATTRIBUTES

protected:
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;


#pragma endregion
#pragma region SECONDARY ATTRIBUTES

protected:
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

#pragma endregion
#pragma region VITAL ATTRIBUTES

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;
	
#pragma endregion

#pragma endregion

#pragma endregion

};
