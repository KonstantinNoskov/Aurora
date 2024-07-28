#include "AbilitySystem/Abilities/AuroraProjectileSpell.h"

#include "Actor/AuroraProjectile.h"
#include "Interfaces/Interaction/CombatInterface.h"


void UAuroraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                             const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const bool bIsServer = HasAuthority(&ActivationInfo); 
	
	if (!bIsServer) return;

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);

		// TODO: Set the Projectile Rotation
		
		AAuroraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuroraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
			);

		// TODO: Give the projectile a gameplay effect spec for causing damage.
		
		Projectile->FinishSpawning(SpawnTransform);
	}
}
