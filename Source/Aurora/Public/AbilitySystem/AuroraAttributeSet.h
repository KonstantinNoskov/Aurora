﻿#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuroraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

class UAttributeSet;

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties() {};
	
	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
	
	UPROPERTY()
	FGameplayEffectContextHandle EffectContextHandle;
};

// Static Function pointer template
template<class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;


UCLASS()
class AURORA_API UAuroraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	UAuroraAttributeSet();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	void HandelIncomingXP(const FEffectProperties& Props);
	void HandleIncomingDamage(const FEffectProperties& Props);
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
		
	// [GameplayTag - Gameplay Attribute] Map
	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes;

private:

	/**
	 *  Fills up FEffectProperties struct.
	 *
	 *  @param Data - contains different info about EffectSpec, EvaluatedData, Target.
	 *  @param Props - FEffectProperties struct which is stores all info actors, pawns, controllers, ability systems etc.
	 */
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;

	/**
	 * 
	 */
	void ShowFloatingText(const FEffectProperties& Props, const float Damage, bool bBlockedHit, bool bCriticalHit) const;

	/**
	 * 
	 */
	void TakenDamageHandle(const FEffectProperties& Props, const float NewHealth);

	/**
	 * 
	 */
	void SendXPEvent(const FEffectProperties& Props);

	/**
	 * 
	 */
	void Debuff(const FEffectProperties& InProps);

#pragma region VITAL ATTRIBUTES

public:

	ATTRIBUTE_ACCESSORS(UAuroraAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UAuroraAttributeSet, Mana);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes")
	FGameplayAttributeData Mana;
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

#pragma endregion
#pragma region PRIMARY ATTRIBUTES

	ATTRIBUTE_ACCESSORS(UAuroraAttributeSet, Strength);
	ATTRIBUTE_ACCESSORS(UAuroraAttributeSet, Intelligence);
	ATTRIBUTE_ACCESSORS(UAuroraAttributeSet, Resilience);
	ATTRIBUTE_ACCESSORS(UAuroraAttributeSet, Vigor);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Primary Attributes")
	FGameplayAttributeData Strength;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Primary Attributes")
	FGameplayAttributeData Intelligence;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience, Category = "Primary Attributes")
	FGameplayAttributeData Resilience;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "Primary Attributes")
	FGameplayAttributeData Vigor;
	
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;
	
	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;
	
	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;
	
	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;

#pragma endregion
#pragma region SECONDARY ATTRIBUTES

	ATTRIBUTE_ACCESSORS(UAuroraAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UAuroraAttributeSet, MaxMana);
	ATTRIBUTE_ACCESSORS(UAuroraAttributeSet, Armor);
	ATTRIBUTE_ACCESSORS(UAuroraAttributeSet, ArmorPenetration);
	ATTRIBUTE_ACCESSORS(UAuroraAttributeSet, BlockChance);
	ATTRIBUTE_ACCESSORS(UAuroraAttributeSet, CriticalHitChance);
	ATTRIBUTE_ACCESSORS(UAuroraAttributeSet, CriticalHitDamage);
	ATTRIBUTE_ACCESSORS(UAuroraAttributeSet, CriticalHitResistance);
	ATTRIBUTE_ACCESSORS(UAuroraAttributeSet, HealthRegeneration);
	ATTRIBUTE_ACCESSORS(UAuroraAttributeSet, ManaRegeneration);


	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Health Attributes")
	FGameplayAttributeData MaxHealth;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Mana Attributes")
	FGameplayAttributeData MaxMana;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Secondary Attributes")
	FGameplayAttributeData Armor;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category = "Secondary Attributes")
	FGameplayAttributeData ArmorPenetration;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category = "Secondary Attributes")
	FGameplayAttributeData BlockChance;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitChance;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamage, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitDamage;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitResistance, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitResistance;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "Secondary Attributes")
	FGameplayAttributeData HealthRegeneration;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "Secondary Attributes")
	FGameplayAttributeData ManaRegeneration;

	
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
	
	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;
	
	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const;
	
	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;
	
	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;
	
	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const;
	
	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const;
	
	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;
	
	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;

#pragma endregion
#pragma region Damage Resistance Attributes

	ATTRIBUTE_ACCESSORS(UAuroraAttributeSet, FireResistance);
	ATTRIBUTE_ACCESSORS(UAuroraAttributeSet, LightningResistance);
	ATTRIBUTE_ACCESSORS(UAuroraAttributeSet, ArcaneResistance);
	ATTRIBUTE_ACCESSORS(UAuroraAttributeSet, PhysicalResistance);
	
	// Fire
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "Resistance Attributes")
	FGameplayAttributeData FireResistance;

	// Lightning
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "Resistance Attributes")
	FGameplayAttributeData LightningResistance;

	// Arcane
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "Resistance Attributes")
	FGameplayAttributeData ArcaneResistance;

	// Physical
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "Resistance Attributes")
	FGameplayAttributeData PhysicalResistance;
	
	
	UFUNCTION()
	void OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const;
	
	UFUNCTION()
	void OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const;
	
	UFUNCTION()
	void OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance) const;
	
	UFUNCTION()
	void OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const;

#pragma endregion
#pragma region Meta Attributes

	ATTRIBUTE_ACCESSORS(UAuroraAttributeSet, IncomingDamage)
	ATTRIBUTE_ACCESSORS(UAuroraAttributeSet, IncomingXP)

	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage;

	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingXP;
	
	
#pragma endregion


private:

	bool bTopOffHealth = false;
	bool bTopOffMana = false;
};














