#include "AbilitySystem/Abilities/ArcaneShards.h"

FString UArcaneShards::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);
	
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			// TITLE
			"<AbilityName.Arcane>ARCANE SHARDS</>\n\n"

			// Requirements
			"<Level>Level: %d</>\n" // Level
			"<Manacost>Manacost: %1.f</>\n" // ManaCost
			"<Cooldown>Cooldown: %.1f</>\n\n" // Cooldown

			// Details
			"<Default>Summon shard of arcane energy, "
			"causing radial arcane damage of: "

			// Damage
			"</><DamageType.Arcane>%d</>"
			"<Default> at the shard origin</>\n\n"
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
			"<AbilityName.Arcane>ARCANE SHARDS</>\n\n"

			// Requirements
			"<Level>Level: %d</>\n" // Level
			"<Manacost>Manacost: %.1f</>\n" // ManaCost
			"<Cooldown>Cooldown: %.1f</>\n\n" // Cooldown

			// Details
			"<Default>Summon %d shards of arcane energy, "
			"causing radial arcane damage of: "

			// Damage
			"</><DamageType.Arcane>%d</><Default> at the shard origin</>\n\n"
			),

			// Values
			Level,
			FMath::Abs(ManaCost),
			Cooldown,
			FMath::Min(Level,MaxNumShards),
			ScaledDamage);
	}
}

FString UArcaneShards::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
			// TITLE
			"<AbilityName.Arcane>NEXT LEVEL</>\n\n"

			// Requirements
			"<Level>Level: %d</>\n" // Level
			"<Manacost>Manacost: %.1f</>\n" // ManaCost
			"<Cooldown>Cooldown: %.1f</>\n\n" // Cooldown

			// Details
			"<Default>Summon %d shards of arcane energy, "
			"causing radial arcane damage of: "

			// Damage
			"</><DamageType.Arcane>%d</><Default> at the shard origin</>\n\n"
			),

			// Values
			Level,
			FMath::Abs(ManaCost),
			Cooldown,
			FMath::Min(Level,MaxNumShards),
			ScaledDamage);
}
