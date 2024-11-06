#include "AbilitySystem/Abilities/AuroraGameplayAbility.h"

#include "AbilitySystem/AuroraAttributeSet.h"


#pragma region Ability Description Format

FString UAuroraGameplayAbility::GetDescription(int32 Level)
{
	return FString::Printf(TEXT(
		"<Default>%s, </><Level>%d</>"
		),
		L"Default Ability Name - LoremIpsum LoremIpsum LoremIpsum", Level);
}

FString UAuroraGameplayAbility::GetNextLevelDescription(int32 Level)
{
	return FString::Printf(TEXT(
		"<Default>Next Level: </><Level>%d</> \n"
		"<Default>Causes much more damage. </>"
		),
		Level);
}

FString UAuroraGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT(
		"<Default>Spell Locked Until Level: %d</>"
		),
		Level);
}



float UAuroraGameplayAbility::GetManaCost(float InLevel) const
{
	float ManaCost = 0.f;
	if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for (FGameplayModifierInfo Mod : CostEffect->Modifiers)
		{
			if (Mod.Attribute == UAuroraAttributeSet::GetManaAttribute())
			{
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, ManaCost);
				break;
			}
		}
	}

	return ManaCost;
}

float UAuroraGameplayAbility::GetCooldown(float InLevel) const
{
	float Cooldown = 0.f;
	if (const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, Cooldown);
	}

	
	return Cooldown;
}

#pragma endregion



