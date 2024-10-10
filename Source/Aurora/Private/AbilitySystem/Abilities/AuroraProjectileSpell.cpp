#include "AbilitySystem/Abilities/AuroraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuroraProjectile.h"
#include "Interfaces/Interaction/CombatInterface.h"

void UAuroraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                             const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAuroraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride)
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
	
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());
	
	// Spawn projectile
	AAuroraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuroraProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetAvatarActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);

	//Projectile->ProjectileBehaviorParams = MakeProjectileBehaviorParams();
	Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
	
	Projectile->FinishSpawning(SpawnTransform);
}
