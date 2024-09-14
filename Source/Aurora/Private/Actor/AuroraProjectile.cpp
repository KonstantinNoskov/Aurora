#include "Actor/AuroraProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuroraAbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/AuroraGameplayAbility.h"
#include "Aurora/Aurora.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Debug/DebugMacros.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


AAuroraProjectile::AAuroraProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);

	// Collision
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	// Projectile Movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 500.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AAuroraProjectile::BeginPlay()
{
	Super::BeginPlay();

	// Set Projectile life time
	SetLifeSpan(LifeSpan);
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuroraProjectile::OnSphereOverlap);
	
	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
}

void AAuroraProjectile::Destroyed()
{
	if (!bHit && !HasAuthority())
	{
		// Play Sound
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);

		// Apply Impact Effect
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());

		// Stop looping sound
		if (LoopingSoundComponent)
		{
			LoopingSoundComponent->Stop();
		}

		bHit = true;
	}
	
	Super::Destroyed();
}

void AAuroraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	
	if (!DamageEffectSpecHandle.Data.IsValid() || DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser() == OtherActor)
	{
		return;
	}
		

	if (UAuroraAbilitySystemLibrary::IsActorsFriendly(DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser(), OtherActor))
	{
		return;
	}
	
	if (!bHit)
	{
		// Play Sound
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);

		// Apply Impact Effect
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());

		// Stop looping sound
		if (LoopingSoundComponent) LoopingSoundComponent->Stop();
		bHit = true;
	}
	
	
	if (HasAuthority())
	{
		
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		}
		
		Destroy();
	}
	else
	{
		bHit = true;
	}
}


