#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuroraPlayerState.generated.h"

class ULevelUpInfo;
// Ability System
class UAttributeSet;


DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangedSignature, int32);

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

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAttributeSet> AttributeSet;

public:
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	FORCEINLINE TObjectPtr<UAttributeSet> GetAttributeSet() const { return AttributeSet; }

#pragma endregion

#pragma region XP & Level UP

public:
	
	FOnPlayerStatChangedSignature OnXPChanged;
	FOnPlayerStatChangedSignature OnLevelChanged;

private:
	
	UFUNCTION()
	void OnRep_Level(int32 OldLevel) const;

	UFUNCTION()
	void OnRep_XP(int32 OldXP) const;

public:
	
	FORCEINLINE int32 GetPlayerLevel_Implementation() const	{ return Level; }
	FORCEINLINE int32 GetXP() const				{ return XP; }
	
	
	void AddToXP(int32 AddXP);
	void AddToLevel(int32 AddLevel);

	void SetXP(int32 NewXP); 
	void SetLevel(int32 NewLevel); 
	

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

private:

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level, Category = "Level")
	int32 Level = 1;
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_XP, Category = "Level")
	int32 XP = 0;

#pragma endregion
	
};
