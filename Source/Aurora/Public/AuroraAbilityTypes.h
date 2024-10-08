#pragma once

#include "GameplayEffectTypes.h"
#include "AuroraAbilityTypes.generated.h"


class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams(){}

	UPROPERTY()
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceASC = nullptr;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetASC = nullptr;

	UPROPERTY()
	float BaseDamage = 0;

	UPROPERTY()
	FGameplayTag DamageType = FGameplayTag();
	
	UPROPERTY()
	float AbilityLevel = 1.f;
	
	UPROPERTY()
	float DebuffChance = 0.f;
	
	UPROPERTY()
	float DebuffDamage = 0.f;

	UPROPERTY()
	float DebuffDuration = 0.f;

	UPROPERTY()
	float DebuffFrequency = 0.f;

	UPROPERTY()
	float DeathImpulseMagnitude = 60.f;

	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector; 
	
};

USTRUCT(BlueprintType)
struct FAuroraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FGameplayEffectContext::StaticStruct();
	}
	
	virtual FAuroraGameplayEffectContext* Duplicate() const override
	{
		FAuroraGameplayEffectContext* NewContext = new FAuroraGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	// Custom serialization 
	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;

#pragma region Custom context properties

#pragma region Blocked Hit

public:
	
	bool IsBlockedHit() const						{ return bIsBlockedHit; }
	void SetIsBlockedHit(bool bInIsBlockedHit)		{ bIsBlockedHit = bInIsBlockedHit; }

protected:

	UPROPERTY()
	bool bIsBlockedHit = false;
	
#pragma endregion
#pragma region Critical Hit

public:
	
	bool IsCriticalHit() const						{ return bIsCriticalHit; }
	void SetIsCriticalHit(bool bInIsCriticalHit)	{ bIsCriticalHit = bInIsCriticalHit; }

protected:
	
	UPROPERTY()
	bool bIsCriticalHit = false;
	
#pragma endregion
#pragma region Debuff props

public:
	
	bool IsDebuffSuccessful() const										{ return bIsSuccessfulDebuff; }
	float GetDebuffDamage() const										{ return DebuffDamage; }
	float GetDebuffDuration() const										{ return DebuffDuration; }
	float GetDebuffFrequency() const									{ return DebuffFrequency; }
	TSharedPtr<FGameplayTag> GetDamageType() const						{ return DamageTypeTag; }

	void SetIsDebuffSuccessfull(bool NewStatus)							{ bIsSuccessfulDebuff = NewStatus; }
	void SetDebuffDamage(float InDamage)								{ DebuffDamage = InDamage; }
	void SetDebuffDuration(float InDuration)							{ DebuffDuration = InDuration; }
	void SetDebuffFrequency(float InFrequency)							{ DebuffFrequency = InFrequency; }
	void SetDamageType(TSharedPtr<FGameplayTag> InDamageTypeTag)	{ DamageTypeTag = InDamageTypeTag; }

protected:
	
	UPROPERTY()
	bool bIsSuccessfulDebuff = false;
	
	UPROPERTY()
	float DebuffDamage = 0.f;

	UPROPERTY()
	float DebuffDuration = 0.f;

	UPROPERTY()
	float DebuffFrequency = 0.f;

	TSharedPtr<FGameplayTag> DamageTypeTag;
	
#pragma endregion
#pragma region Death Impulse

public:

	FVector GetDeathImpulse() const						{ return DeathImpulse; }
	void SetDeathImpulse(const FVector& InDeathImpulse)	{ DeathImpulse = InDeathImpulse; }
	
protected:

	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;

#pragma endregion


#pragma endregion
	
};

/*
 *  This struct is necessary to allow our custom FAuroraGameplayEffectContext be serialized by setting options in enum to true.
 *  To look through all option dive into TStructOpsTypeTraitsBase2. 
 */
template<>struct TStructOpsTypeTraits<FAuroraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FAuroraGameplayEffectContext> 
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true	
	};
};








