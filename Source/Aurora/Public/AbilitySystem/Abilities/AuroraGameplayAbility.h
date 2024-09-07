#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuroraGameplayAbility.generated.h"


UCLASS()
class AURORA_API UAuroraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Debug")
	bool bDebug = false;

public:

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;

	
};
