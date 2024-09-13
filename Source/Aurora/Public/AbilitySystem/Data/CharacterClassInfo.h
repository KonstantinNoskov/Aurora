#pragma once

#include "CoreMinimal.h"
#include "ScalableFloat.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

class UGameplayAbility;
class UGameplayEffect;


UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Elementalist	UMETA(DisplayName = "Elementalist"),
	Warrior			UMETA(DisplayName = "Warrior"),
	Ranger			UMETA(DisplayName = "Ranger"),
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	FScalableFloat XPReward = FScalableFloat();
};

UCLASS()
class AURORA_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	/*
	 * Returns default primary attributes for specific character class (warrior, mage, ranger etc.)
	 */
	UFUNCTION()
	FCharacterClassDefaultInfo GetClassDefaultInfo(ECharacterClass CharacterClass);
	
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInformation;
	
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> VitalAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities; 

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults | Damage", DisplayName = "Attributes Calculation Coefficients")
	TObjectPtr<UCurveTable> AttributesCalcCoefficients;
};
