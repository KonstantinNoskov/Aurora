#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuroraPlayerState.generated.h"

class ULevelUpInfo;
class UAttributeSet;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangedSignature, int32 /*StatValue*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnLevelChanged, int32 /*StatValue*/, bool /*bLevelUp*/);

UCLASS()
class AURORA_API AAuroraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAuroraPlayerState();

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
#pragma region ABILITY SYSTEM

protected:
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UAttributeSet> AttributeSet;

public:
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override ;
	FORCEINLINE TObjectPtr<UAttributeSet> GetAttributeSet() const { return AttributeSet; }

#pragma endregion

#pragma region XP & Level UP

public:
	
	FOnPlayerStatChangedSignature OnXPChanged;
	FOnLevelChanged OnLevelChanged;
	FOnPlayerStatChangedSignature OnAttributePointsChanged;
	FOnPlayerStatChangedSignature OnSpellPointsChanged;

private:
	
	UFUNCTION()
	void OnRep_Level(int32 OldLevel) const;

	UFUNCTION()
	void OnRep_XP(int32 OldXP) const;

	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints) const;

	UFUNCTION()
	void OnRep_SpellPoints(int32 OldSpellPoints) const;

public:
	
	FORCEINLINE int32 GetPlayerLevel() const				{ return Level; }
	FORCEINLINE int32 GetXP() const							{ return XP; }
	FORCEINLINE int32 GetAttributePoints() const			{ return AttributePoints; }
	FORCEINLINE int32 GetSpellPoints() const				{ return SpellPoints; }
	
	void AddToXP(int32 AddXP);
	void AddToLevel(int32 AddLevel);
	void AddToAttributePoints(int32 AddAttributePoints);
	void AddToSpellPoints(int32 AddSpellPoints);

	void SetXP(int32 NewXP); 
	void SetLevel(int32 NewLevel);
	void SetAttributePoints(int32 NewAttributePoints); 
	void SetSpellPoints(int32 NewSpellPoints); 
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

private:

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level, Category = "Level")
	int32 Level = 1;
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_XP, Category = "Level")
	int32 XP = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_XP, Category = "Level")
	int32 AttributePoints = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_XP, Category = "Level")
	int32 SpellPoints = 0;

#pragma endregion
	
};
