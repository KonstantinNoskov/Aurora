﻿#include "AbilitySystem/Abilities/AuroraFireBolt.h"

#include "AbilitySystem/AuroraAbilitySystemLibrary.h"
#include "Actor/AuroraProjectile.h"


FString UAuroraFireBolt::GetDescription(int32 Level)
{
	
	//const int32 Damage = GetDamageByDamageType(Level, FAuroraGameplayTags::Get().Damage_Fire); // Uncomment this for multiple damage types handle
	
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
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
			"<Damage>%d</> <Default> fire damage with chance to burn</>\n\n" // Damage
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

	const float ScaledDamage = Damage.GetValueAtLevel(Level);
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

void UAuroraFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	// Authority check
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority(); 
	if (!bIsServer) return;

	// Get the location where the projectile is gonna be spawn from
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();

	// bOverridePitch == false - Projectile can be shoot to the floor.
	// bOverridePitch == true && PitchOverride == 0 - Z-axis is locked and projectile fly straight forward 
	Rotation.Pitch = bOverridePitch ? PitchOverride : Rotation.Pitch;
	
	const FVector Forward = Rotation.Vector();

	int32 ProjectilesToSpawn = GetAbilityLevel();
	
	TArray<FRotator> Rotations = UAuroraAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, ProjectilesToSpawn);

	for (const FRotator& Rot : Rotations)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rot.Quaternion());

		// Spawn projectile
		AAuroraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuroraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetAvatarActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
			);
	
		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
	
		Projectile->FinishSpawning(SpawnTransform);
	}
	
	/*// Projectile Forward
	UKismetSystemLibrary::DrawDebugArrow(
		GetAvatarActorFromActorInfo(),
		SocketLocation, 
		SocketLocation + Forward * 100.f,
		1,		
		FLinearColor::White,	
		120,					
		1						
		);

	// Right of Spread
	UKismetSystemLibrary::DrawDebugArrow(
		GetAvatarActorFromActorInfo(),
		SocketLocation, 
		SocketLocation + RightOfSpread * 100.f,
		1,		
		FLinearColor::Green,	
		120,					
		1						
		);

	// Left of Spread 
	UKismetSystemLibrary::DrawDebugArrow(
		GetAvatarActorFromActorInfo(),
		SocketLocation, 
		SocketLocation + LeftOfSpread * 100.f,
		1,		
		FLinearColor::Blue,	
		120,					
		1						
		);*/
}