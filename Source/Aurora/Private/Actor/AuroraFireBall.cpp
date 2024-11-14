#include "Actor/AuroraFireBall.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuroraGameplayTags.h"
#include "GameplayCueManager.h"
#include "AbilitySystem/AuroraAbilitySystemLibrary.h"
#include "Components/AudioComponent.h"

AAuroraFireBall::AAuroraFireBall()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AAuroraFireBall::BeginPlay()
{
	Super::BeginPlay();

	StartOutgoingTimeLline();
}

void AAuroraFireBall::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Overlap valid check
	if (!IsValidOverlap(OtherActor)) return;
	
	if (HasAuthority())
	{
		
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
		if (TargetASC)
		{
			// Pass in DamageEffectParams...

			// Calculate Death impulse direction
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;

			// Death impulse vector
			DamageEffectParams.DeathImpulse = DeathImpulse;

		

			// Target ability system component
			DamageEffectParams.TargetASC = TargetASC;		
			UAuroraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}
	}
}

void AAuroraFireBall::OnHit()
{
	//Super::OnHit();
	if (GetOwner())
	{
		FGameplayCueParameters GameplayCueParameters;
		GameplayCueParameters.Location = GetActorLocation();
		UGameplayCueManager::ExecuteGameplayCue_NonReplicated(GetOwner(), FAuroraGameplayTags::Get().GameplayCue_FireBlast, GameplayCueParameters); 	
	}
	
	// Stop looping sound
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}

	bHit = true;
}



