#pragma once

#include "GameplayEffectTypes.h"
#include "AuroraAbilityTypes.generated.h"


USTRUCT(BlueprintType)
struct FAuroraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	
	virtual UScriptStruct* GetScriptStruct() const
	{
		return StaticStruct();
	}
	
	virtual FAuroraGameplayEffectContext* Duplicate() const
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








