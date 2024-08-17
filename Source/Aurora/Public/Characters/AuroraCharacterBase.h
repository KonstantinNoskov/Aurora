#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/Interaction/CombatInterface.h"
#include "AuroraCharacterBase.generated.h"

// Ability System
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;


UCLASS(Abstract)
class AURORA_API AAuroraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()
	
public:
	
	AAuroraCharacterBase();

protected:

	virtual void BeginPlay() override;

protected:

#pragma region COMBAT
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName;
	
	virtual FVector GetCombatSocketLocation() override;

	

#pragma endregion

#pragma region DISSOLVE EFFECT
	
	void Dissolve();

	/*
	 * Set Timeline to dissolve character mesh after death
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeLine(UMaterialInstanceDynamic* DynamicMaterialInstance);

	/*
	 * Set Timeline to dissolve character's weapon mesh after death
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeLine(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;
	
#pragma endregion
	
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
	virtual void InitializeDefaultAttributes() const;

public:

	UFUNCTION()
	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, const float Level) const;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	FORCEINLINE TObjectPtr<UAttributeSet> GetAttributeSet() const { return AttributeSet; }

#pragma region COMBAT 
	
	virtual void Die() override;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();
	
	FORCEINLINE virtual UAnimMontage* GetHitReactMontage_Implementation() override { return HitReactMontage; }
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

#pragma endregion

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
