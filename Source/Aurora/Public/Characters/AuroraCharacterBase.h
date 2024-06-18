#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AuroraCharacterBase.generated.h"

UCLASS(Abstract)
class AURORA_API AAuroraCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	
	AAuroraCharacterBase();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

};
