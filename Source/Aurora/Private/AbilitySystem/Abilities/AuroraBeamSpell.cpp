#include "AbilitySystem/Abilities/AuroraBeamSpell.h"

#include "InputState.h"
#include "AbilitySystem/AuroraAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

void UAuroraBeamSpell::StoreMouseDataInfo(const FHitResult& InHitResult)
{
	if (InHitResult.bBlockingHit)
	{
		MouseHitLocation = InHitResult.ImpactPoint;
		MouseHitActor = InHitResult.GetActor();
	}
	else
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
	
}

void UAuroraBeamSpell::StoreOwnerPlayerVariables()
{
	if (CurrentActorInfo)
	{
		OwnerPC = CurrentActorInfo->PlayerController.Get();
		OwnerCharacter = Cast<ACharacter>(GetActorInfo().AvatarActor);
	}
}

void UAuroraBeamSpell::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	check(OwnerCharacter)
	if (OwnerCharacter->Implements<UCombatInterface>())
	{
		if (USkeletalMeshComponent* Weapon = ICombatInterface::Execute_GetWeapon(OwnerCharacter))
		{
			TArray<AActor*> ActorsToIgnore;
			ActorsToIgnore.Add(OwnerCharacter);
			FHitResult HitResult;
			
			
			const FVector SocketLocation = Weapon->GetSocketLocation(FName("TipSocket"));
			
			UKismetSystemLibrary::SphereTraceSingle(
				OwnerCharacter, 
				SocketLocation,
				BeamTargetLocation, 
				10.f, 
				TraceTypeQuery1,
				false,
				ActorsToIgnore,
				EDrawDebugTrace::None,
				HitResult,
				true
				);

			if (HitResult.bBlockingHit)
			{
				MouseHitLocation = HitResult.ImpactPoint;
				MouseHitActor = HitResult.GetActor();
			}
		}
	}

	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(MouseHitActor))
	{
		if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UAuroraBeamSpell::PrimaryTargetDied))
		{
			CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UAuroraBeamSpell::PrimaryTargetDied);	
		}
		
	}
}

void UAuroraBeamSpell::StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets)
{
	// Storage
	TArray<AActor*> OverlappingActors;

	// Actors to Ignore
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	ActorsToIgnore.Add(MouseHitActor);
	
	UAuroraAbilitySystemLibrary::GetAlivePlayersWithinRadius(
		GetAvatarActorFromActorInfo(),
		OverlappingActors,
		ActorsToIgnore,
		850.f,
		MouseHitActor->GetActorLocation()
	);

	//int32 NumAdditionalTargets = FMath::Min(GetAbilityLevel() - 1, MaxNumShockTargets);
	int32 NumAdditionalTargets = 5;

	UAuroraAbilitySystemLibrary::GetClosestTargets(
		MouseHitActor->GetActorLocation(),
		NumAdditionalTargets,
		OverlappingActors,
		OutAdditionalTargets);
	
	for (AActor* Target : OutAdditionalTargets)
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Target))
		{
			if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UAuroraBeamSpell::AdditionalTargetDied))
			{
				CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UAuroraBeamSpell::AdditionalTargetDied);	
			}
		}
	}
}
