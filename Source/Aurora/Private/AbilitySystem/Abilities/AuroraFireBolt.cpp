#include "AbilitySystem/Abilities/AuroraFireBolt.h"

#include "AbilitySystem/AuroraAbilitySystemLibrary.h"
#include "Actor/AuroraProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"


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
			"<AbilityName.Fire>FIRE BOLT</>\n\n"
			
			// Requirements
			"<Level>Level: %d</>\n" // Level
			"<Manacost>Manacost: %1.f</>\n" // ManaCost
			"<Cooldown>Cooldown: %.1f</>\n\n" // Cooldown

			// Details
			"<Default>Launches a bolt of fire, "
			"exploding on impact and dealing: </><DamageType.Fire>%d</>" // Damage
			"<Default>fire damage with chance to burn</>\n\n"
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
			"<AbilityName.Fire>FIRE BOLT</>\n\n"

			// Requirements
			"<Level>Level: %d</>\n" // Level
			"<Manacost>Manacost: %.1f</>\n" // ManaCost
			"<Cooldown>Cooldown: %.1f</>\n\n" // Cooldown

			// Details
			"<Default>Launches %d bolts of fire, exploding on impact and dealing: </>" // ProjectilesNum 
			"<DamageType.Fire>%d</> <Default> fire damage with chance to burn</>\n\n" // Damage
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

	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);
	
	return FString::Printf(
		// TITLE
		TEXT("<AbilityName.Fire>NEXT LEVEL</>\n\n"

		// Requirements
		"<Level>Level: %d</>\n" // Level
		"<Manacost>Manacost: %.1f</>\n" // ManaCost
		"<Cooldown>Cooldown: %.1f</>\n\n" // Cooldown

		// Details
		"<Default>Launches %d bolts of fire, " // ProjectilesNum 
		"exploding on impact and dealing: </><DamageType.Fire>%d</>" // Damage
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

	const int32 EffectiveProjectilesNum = FMath::Min(ProjectilesNum, GetAbilityLevel());
	
	TArray<FRotator> Rotations = UAuroraAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, EffectiveProjectilesNum);

	FProjectileBehaviorParams ProjectileBehaviorParam = MakeProjectileBehaviorParams(HomingTarget, ProjectileTargetLocation);
	FDamageEffectParams DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
	
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

		Projectile->ProjectileBehaviorParams = ProjectileBehaviorParam;
		Projectile->DamageEffectParams = DamageEffectParams;

		if (bHoming)
		{
			if (HomingTarget && HomingTarget->Implements<UCombatInterface>())
			{
				Projectile->GetProjectileMovement()->HomingTargetComponent = HomingTarget->GetRootComponent();
			}

			else
			{
				Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
				Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocation);
				Projectile->GetProjectileMovement()->HomingTargetComponent = Projectile->HomingTargetSceneComponent;
			}

			Projectile->GetProjectileMovement()->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);
			Projectile->GetProjectileMovement()->bIsHomingProjectile = true;

			// Debug
			if (bDebug)
			{
				
				GEngine->AddOnScreenDebugMessage(1, 120.f, FColor::Green, FString::Printf(
				TEXT("[%hs]: "
					"Activated after %f."
					"bHoming = %hhd "
					"HomingTarget = %s "
					"MinAcceleration = %f "
					"MaxAccleration = %f"
					"TargetLocation = %s"),
					__FUNCTION__,
					ProjectileActivationSpan,
					bHoming,
					*HomingTarget->GetName(),
					HomingAccelerationMin,
					HomingAccelerationMax,
					*ProjectileTargetLocation.ToString()
					));
			}
		}
		
		Projectile->FinishSpawning(SpawnTransform);
	}
}


