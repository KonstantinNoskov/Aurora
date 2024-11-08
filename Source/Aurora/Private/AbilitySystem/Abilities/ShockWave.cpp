#include "AbilitySystem/Abilities/ShockWave.h"

FString UShockWave::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);
	
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			// TITLE
			"<Title>SHOCK WAVE</>\n\n"

			// Requirements
			"<Level>Level: %d</>\n" // Level
			"<Manacost>Manacost: %1.f</>\n" // ManaCost
			"<Cooldown>Cooldown: %.1f</>\n\n" // Cooldown

			// Details
			"<Default>Emits a beam of lightning, "
			"connecting with the target, repeatedly causing: "

			// Damage
			"</><Damage>%d</>"
			"<Default> lightning damage with chance to shock</>\n\n"
			),

			// Values
			Level,
			FMath::Abs(ManaCost),
			Cooldown,
			ScaledDamage);
	}
	else
	{
		return FString::Printf(TEXT(
			// TITLE
			"<Title>SHOCK WAVE</>\n\n"

			// Requirements
			"<Level>Level: %d</>\n" // Level
			"<Manacost>Manacost: %.1f</>\n" // ManaCost
			"<Cooldown>Cooldown: %.1f</>\n\n" // Cooldown

			// Details
			"<Default>Emits a beam of lightning, "
			"propagating to %d additional targets nearby, causing: "

			// Damage
			"</><Damage>%d</>"
			"<Default> lightning damage with chance to shock</>\n\n"
			),

			// Values
			Level,
			FMath::Abs(ManaCost),
			Cooldown,
			FMath::Min(Level,MaxNumShockTargets - 1),
			ScaledDamage);
	}
}

FString UShockWave::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
			// TITLE
			"<Title>NEXT LEVEL</>\n\n"

			// Requirements
			"<Level>Level: %d</>\n" // Level
			"<Manacost>Manacost: %.1f</>\n" // ManaCost
			"<Cooldown>Cooldown: %.1f</>\n\n" // Cooldown

			// Details
			"<Default>Emits a beam of lightning, "
			"propagating to %d additional targets nearby, causing: "

			// Damage
			"</><Damage>%d</>"
			"<Default> lightning damage with chance to shock</>\n\n"
			),

			// Values
			Level,
			FMath::Abs(ManaCost),
			Cooldown,
			FMath::Min(Level,MaxNumShockTargets - 1),
			ScaledDamage);
}
