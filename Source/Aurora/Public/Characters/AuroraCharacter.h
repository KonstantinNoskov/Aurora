#pragma once

#include "CoreMinimal.h"
#include "AuroraCharacterBase.h"
#include "Interfaces/Interaction/PlayerInterface.h"
#include "AuroraCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class AURORA_API AAuroraCharacter : public AAuroraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

public:
	AAuroraCharacter();
	
#pragma region Player Interface overriden functions

public:

	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void LevelUp_Implementation() override;

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

#pragma region COMBAT INTERFACE

	UFUNCTION()
	virtual int32 GetPlayerLevel_Implementation() override;
	
#pragma endregion
};
