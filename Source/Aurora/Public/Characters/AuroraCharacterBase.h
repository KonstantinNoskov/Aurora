#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/Interaction/CombatInterface.h"
#include "AuroraCharacterBase.generated.h"

// VFX
class UNiagaraSystem;

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

#pragma region COMBAT

public:
	
	virtual void Die() override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& InMontageTag) override;
	
	FORCEINLINE virtual UAnimMontage* GetHitReactMontage_Implementation() const				override	{ return HitReactMontage; }
	FORCEINLINE virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() const		override	{ return AttackMontages; }
	FORCEINLINE virtual float GetMeleeAttackRadius_Implementation() const					override	{ return MeleeAttackRadius; }
	FORCEINLINE virtual UNiagaraSystem* GetBloodEffect_Implementation() const				override	{ return BloodEffect; };
	FORCEINLINE virtual AActor* GetAvatar_Implementation()									override	{ return this; }
	FORCEINLINE virtual bool IsDead_Implementation() const									override	{ return bDead; }
	

protected:
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	// Sockets
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName RightHandSocketName;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName LeftHandSocketName;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY()
	bool bDead = false;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FTaggedMontage> AttackMontages;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float MeleeAttackRadius = 40.f;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	UNiagaraSystem* BloodEffect;

#pragma endregion

#pragma region DISSOLVE EFFECT

protected:
	
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
