#pragma once

#include "CoreMinimal.h"
#include "AuroraCharacterBase.h"
#include "AuroraCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class AURORA_API AAuroraCharacter : public AAuroraCharacterBase
{
	GENERATED_BODY()

public:
	AAuroraCharacter();

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
	virtual int32 GetPlayerLevel() override;
	
#pragma endregion
};
