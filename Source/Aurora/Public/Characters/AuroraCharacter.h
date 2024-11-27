#pragma once

#include "CoreMinimal.h"
#include "AuroraCharacterBase.h"
#include "Interfaces/Interaction/PlayerInterface.h"
#include "Player/AuroraPlayerState.h"
#include "AuroraCharacter.generated.h"

class UNiagaraComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class AURORA_API AAuroraCharacter : public AAuroraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

public:
	AAuroraCharacter();

#pragma region Combat Interface overriden functions

	UFUNCTION()
	virtual int32 GetPlayerLevel_Implementation() override;
	
#pragma endregion
#pragma region Player Interface overriden functions

public:

	// XP
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual int32 GetXP_Implementation() const override;

	// Level
	virtual void LevelUp_Implementation() override;
	virtual int32 FindLevelForXP_Implementation(int32 InXP) const override;
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;
	
	// Attribute Points
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;
	virtual int32 GetAttributePointsReward_Implementation(int32 Level) const override;
	virtual int32 GetAttributePoints_Implementation() const override;

	// Spell Points
	virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;
	void AssignWidgetControllers(AAuroraPlayerState* AuroraPlayerState);
	virtual int32 GetSpellPointsReward_Implementation(int32 Level) const override;
	virtual int32 GetSpellPoints_Implementation() const override;

	// Cursor Magic Circle
	virtual AMagicCircle* ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial, float InMagicCircleRadius) override;
	virtual void HideMagicCircle_Implementation() override;

	// Save Progress
	virtual void SaveProgress_Implementation(const FName& CheckPointTag) override;

#pragma endregion

protected:
	virtual void BeginPlay() override;

public:
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

#pragma region CAMERA

private:
	
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> AuroraSpringArmComponent;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> AuroraCameraComponent;

#pragma endregion
#pragma region ABILITY SYSTEM

	UFUNCTION()
	virtual void InitAbilityActorInfo() override;

#pragma endregion
	
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastLevelUpParticles() const;

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;
	
};
