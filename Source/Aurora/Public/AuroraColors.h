#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AuroraColors.generated.h"

UCLASS()
class AURORA_API UAuroraColors : public UDataAsset
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	FColor FindColorByTag(const FGameplayTag& GameplayTag, bool bLogNotFound = false) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, DisplayName = "Damage Type")
	TMap<FGameplayTag, FColor> DamageTypeToColor;
	
};
