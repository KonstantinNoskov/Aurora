#include "AbilitySystem/Abilities/AuroraGameplayAbility.h"


#pragma region Ability Description Format

FString UAuroraGameplayAbility::GetDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"), L"Default Ability Name - LoremIpsum LoremIpsum LoremIpsum", Level);
}

FString UAuroraGameplayAbility::GetNextLevelDescription(int32 Level)
{
	return FString::Printf(
		TEXT("<Default>Next Level: </><Level>%d</> \n<Default>Causes much more damage. </>"), Level);
}

FString UAuroraGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Spell Locked Until Level: %d</>"), Level);
}

#pragma endregion



