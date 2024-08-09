#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuroraGameplayAbility.generated.h"


UCLASS()
class AURORA_API UAuroraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag; 
};
