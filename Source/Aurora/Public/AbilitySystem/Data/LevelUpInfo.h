#pragma once

#include "CoreMinimal.h"
#include "ScalableFloat.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

USTRUCT(BlueprintType)
struct FAuroraLevelUpInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FScalableFloat LevelUpRequirementScale = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 LevelUpRequirement = 0;
	
	UPROPERTY(EditDefaultsOnly)
	int32 AttributePointReward = 1;

	UPROPERTY(EditDefaultsOnly)
	int32 SpellPointReward = 1;
};

UCLASS()
class AURORA_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TArray<FAuroraLevelUpInfo> LevelUpInformation;

	UPROPERTY(EditDefaultsOnly)
	FAuroraLevelUpInfo LevelUpInfo;

	int32 FindLevelForXP(int32 XP);
	int32 FindLevelForXPByCurve(int32 XP) const;
};
