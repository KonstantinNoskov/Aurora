﻿#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuroraGameModeBase.generated.h"

class UAbilityInfo;
class UCharacterClassInfo;

UCLASS()
class AURORA_API AAuroraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	
};
