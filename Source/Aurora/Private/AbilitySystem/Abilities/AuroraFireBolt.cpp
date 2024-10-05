#include "AbilitySystem/Abilities/AuroraFireBolt.h"

FString UAuroraFireBolt::GetDescription(int32 Level)
{
	
	//const int32 Damage = GetDamageByDamageType(Level, FAuroraGameplayTags::Get().Damage_Fire); // Uncomment this for multiple damage types handle
	
	const float ScaledDamage = DamageValue.GetValueAtLevel(Level);
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
			"exploding on impact and dealing: </><Damage>%f</>" // Damage
			"<Default> fire damage with chance to burn</>\n\n"
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
			"<Title>FIRE BOLT</>\n\n"

			// Requirements
			"<Level>Level: %d</>\n" // Level
			"<Manacost>Manacost: %.1f</>\n" // ManaCost
			"<Cooldown>Cooldown: %.1f</>\n\n" // Cooldown

			// Details
			"<Default>Launches %d bolts of fire, exploding on impact and dealing: </>" // ProjectilesNum 
			"<Damage>%f</> <Default> fire damage with chance to burn</>\n\n" // Damage
			),

			// Values
			Level,
			FMath::Abs(ManaCost),
			Cooldown,
			FMath::Min(Level,ProjectilesNum),
			ScaledDamage);
	}
}

FString UAuroraFireBolt::GetNextLevelDescription(int32 Level)
{
	//const int32 Damage = GetDamageByDamageType(Level, FAuroraGameplayTags::Get().Damage_Fire); // Uncomment this for multiple damage types handle

	const float ScaledDamage = DamageValue.GetValueAtLevel(Level);
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
		"exploding on impact and dealing: </><Damage>%f</>" // Damage
		"<Default> fire damage with chance to burn</>\n\n"
		),

		// Values
		Level,
		FMath::Abs(ManaCost),
		Cooldown,
		FMath::Min(Level, ProjectilesNum),
		ScaledDamage);
}
