#include "AbilitySystem/Abilities/AuroraFireBlast.h"

#include "AbilitySystem/AuroraAbilitySystemLibrary.h"
#include "Actor/AuroraFireBall.h"
#include "Aurora/AuroraLogChannels.h"

FString UAuroraFireBlast::GetDescription(int32 Level)
{
	
	//const int32 Damage = GetDamageByDamageType(Level, FAuroraGameplayTags::Get().Damage_Fire); // Uncomment this for multiple damage types handle
	
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
		// TITLE
		"<AbilityName.Fire>FIRE BLAST</>\n\n"
		
		// Requirements
		"<Level>Level: %d</>\n" // Level
		"<Manacost>Manacost: %1.f</>\n" // ManaCost
		"<Cooldown>Cooldown: %.1f</>\n\n" // Cooldown

		// Details
		"<Default>Launches %d </>"
		"<Default>fire balls in all directions, each coming back and </>"
		"<Default>exploding upon return, causing </>"

		// Damage
		"<DamageType.Fire>%d </>" 
		"<Default>fire damage with chance to burn</>\n\n"
		),

		// Values
		Level,
		FMath::Abs(ManaCost),
		Cooldown,
		NumFireBalls,
		ScaledDamage);
	
}
FString UAuroraFireBlast::GetNextLevelDescription(int32 Level)
{
	return GetDescription(Level);
}

TArray<AAuroraFireBall*> UAuroraFireBlast::SpawnFireBalls()
{
	TArray<AAuroraFireBall*> FireBalls;
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	
	TArray<FRotator> Rotators = UAuroraAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, 360.f, NumFireBalls);
	
	for (const FRotator& Rotator : Rotators)
	{
		
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(Rotator.Quaternion());
		
		AAuroraFireBall* FireBall = GetWorld()->SpawnActorDeferred<AAuroraFireBall>(
			FireBallClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			CurrentActorInfo->PlayerController->GetPawn(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
			);

		// Set Avatar Character as the owner
		FireBall->SetOwner(GetAvatarActorFromActorInfo());

		/* Set damage effect params for Fireball target overlap */
		FireBall->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		
		/* Set damage effect params special for Fireball explosion */
		FireBall->ExplosionDamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		
		/* Set Actor that projectile should come back to after crossing apex distance */
		FireBall->ReturnToActor = GetAvatarActorFromActorInfo();

		
		
		FireBalls.Add(FireBall);
		
		FireBall->FinishSpawning(SpawnTransform);
	}
	
	return FireBalls;
}
