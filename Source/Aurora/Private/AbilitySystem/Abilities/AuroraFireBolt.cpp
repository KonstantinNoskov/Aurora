#include "AbilitySystem/Abilities/AuroraFireBolt.h"

#include "AuroraGameplayTags.h"

FString UAuroraFireBolt::GetDescription(int32 Level)
{
	const int32 Damage = GetDamageByDamageType(Level, FAuroraGameplayTags::Get().Damage_Fire);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);
	
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			// TITLE
			"<Title>FIRE BOLT</>\n\n"

			// Requirements
			"<Level>Level: %d</>\n" // Level
			"<Manacost>Manacost: %1.f</>\n" // ManaCost
			"<Cooldown>Cooldown: %.1f</>\n\n" // Cooldown

			// Details
			"<Default>Launches a bolt of fire, "
			"exploding on impact and dealing: </><Damage>%d</>" // Damage
			"<Default> fire damage with chance to burn</>\n\n"
			),

			// Values
			Level,
			FMath::Abs(ManaCost),
			Cooldown,
			Damage);
	}
	else
	{
		return FString::Printf(TEXT(
			// TITLE
			"<Title>FIRE BOLT</>\n\n"

			// Requirements
			"<Level>Level: %d</>\n" // Level
			"<Manacost>Manacost: %.1f</>\n" // ManaCost
			"<Cooldown>Cooldown: %.1f</>\n\n" // Cooldown

			// Details
			"<Default>Launches %d bolts of fire, exploding on impact and dealing: </>" // ProjectilesNum 
			"<Damage>%d</> <Default> fire damage with chance to burn</>\n\n" // Damage
			),

			// Values
			Level,
			FMath::Abs(ManaCost),
			Cooldown,
			FMath::Min(Level,ProjectilesNum),
			Damage);
	}
}

FString UAuroraFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 Damage = GetDamageByDamageType(Level, FAuroraGameplayTags::Get().Damage_Fire);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);
	
	return FString::Printf(
		// TITLE
		TEXT("<Title>FIRE BOLT</>\n\n"

		// Requirements
		"<Level>Level: %d</>\n" // Level
		"<Manacost>Manacost: %.1f</>\n" // ManaCost
		"<Cooldown>Cooldown: %.1f</>\n\n" // Cooldown

		// Details
		"<Default>Launches %d bolts of fire, " // ProjectilesNum 
		"exploding on impact and dealing: </><Damage>%d</>" // Damage
		"<Default> fire damage with chance to burn</>\n\n"
		),

		// Values
		Level,
		FMath::Abs(ManaCost),
		Cooldown,
		FMath::Min(Level, ProjectilesNum),
		Damage);
}
