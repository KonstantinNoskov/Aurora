#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AbilityInfo.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct FAuroraAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag CooldownTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTypeTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag AbilityStatusTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 LevelRequirement = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> Ability;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> Icon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UMaterialInterface> BackgroundMaterial = nullptr;
};

UCLASS()
class AURORA_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInformation", DisplayName = "Abilities")
	TArray<FAuroraAbilityInfo> AbilityInformation;

	FAuroraAbilityInfo FindAbilityInfoByTag(const FGameplayTag& AbilityTag, bool bLogNotFound = false) const;
};
