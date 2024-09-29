#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuroraGameplayAbility.generated.h"


UCLASS()
class AURORA_API UAuroraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()


#pragma region Ability Description Format

#pragma endregion
public:
	
	virtual FString GetDescription(int32 Level);
	virtual FString GetNextLevelDescription(int32 Level);
	
	static FString GetLockedDescription(int32 Level);

protected:
	
	float GetManaCost(float InLevel = 1.f) const;
	float GetCooldown(float InLevel = 1.f) const;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Debug")
	bool bDebug = false;

public:

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;

	
};
