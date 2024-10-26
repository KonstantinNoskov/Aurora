#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interfaces/Interaction/CombatInterface.h"
#include "AuroraCharacterBase.generated.h"

class UPassiveNiagaraComponent;
class UDebuffNiagaraComponent;
enum class ECharacterClass : uint8;

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
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&) const override;

#pragma region Combat Interface overrided functions

public:
	
	virtual void Die														(const FVector& InDeathImpulse)				override;
	virtual FVector GetCombatSocketLocation_Implementation					(const FGameplayTag& MontageTag)			override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation				(const FGameplayTag& InMontageTag)			override;
	virtual void SetMinionCount_Implementation								(int32 NewMinionCount)						override;
	virtual void IncrementMinionCount_Implementation						(int32 Increment)							override;
	virtual FOnASCRegisteredSignature GetOnAscRegisteredDelegate			()											override;
	virtual FOnDeathSignature& GetOnDeathDelegate							()											override;
	virtual const USkeletalMeshSocket* GetCombatSocketByTag_Implementation	(const FGameplayTag& MontageTag)			override;

	FORCEINLINE virtual UAnimMontage* GetHitReactMontage_Implementation() const						override	{ return HitReactMontage; }
	FORCEINLINE virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() const				override	{ return AttackMontages; }
	FORCEINLINE virtual float GetMeleeAttackRadius_Implementation() const							override	{ return MeleeAttackRadius; }
	FORCEINLINE virtual UNiagaraSystem* GetBloodEffect_Implementation() const						override	{ return BloodEffect; };
	FORCEINLINE virtual AActor* GetAvatar_Implementation()											override	{ return this; }
	FORCEINLINE virtual bool IsDead_Implementation() const											override	{ return bDead; }
	FORCEINLINE virtual int32 GetMinionCount_Implementation() const									override	{ return MinionCount; }
	FORCEINLINE virtual ECharacterClass GetCharacterClass_Implementation()							override	{ return CharacterClass; }
	FORCEINLINE virtual USkeletalMeshComponent* GetWeapon_Implementation()							override	{ return Weapon; }
	

#pragma endregion

protected:

	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	// Delegates
	FOnASCRegisteredSignature OnASCRegistered;
	FOnDeathSignature OnDeath;

#pragma region COMBAT

protected:
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(const FVector& InDeathImpulse);

	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
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
	FName TailSocketName;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TMap<FGameplayTag, FName> CombatSockets;
	
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

	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* DeathSound;

	int32 MinionCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 250.f;

public:

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bStunned = false;

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

#pragma region DEFAULT ABILITIES
	
private:
	
	UPROPERTY(EditAnywhere, Category = "Abilitites")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category = "Abilitites")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;

protected:

	UFUNCTION()
	void AddCharacterAbilities();

#pragma endregion

private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> HaloOfProtectionNiagaraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> LifeSiphonNiagaraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> ManaSiphonNiagaraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> EffectAttachComponent;
	
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

protected:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> DebuffNiagaraComponent;



};
