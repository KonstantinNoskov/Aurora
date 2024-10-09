#include "Actor/AuroraProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuroraAbilitySystemLibrary.h"
#include "Aurora/Aurora.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
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

	FTimerHandle ProjectileActivateTimer;
	GetWorld()->GetTimerManager().SetTimer(ProjectileActivateTimer, this, &AAuroraProjectile::OnProjectileActivate, ProjectileBehaviorParams.ActivationSpan);
}

void AAuroraProjectile::OnHit()
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
void AAuroraProjectile::Destroyed()
{
	if (!bHit && !HasAuthority())
	{
		OnHit();
	}
	
	Super::Destroyed();
}
void AAuroraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const AActor* SourceAvatarActor = DamageEffectParams.SourceASC->GetAvatarActor();
	if (SourceAvatarActor == OtherActor) return;
	
	if (UAuroraAbilitySystemLibrary::IsActorsFriendly(SourceAvatarActor, OtherActor)) return;
	
	if (!bHit) OnHit();
	
	if (HasAuthority())
	{
		
		 UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
		if (TargetASC)
		{
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;

			// Pass in  DamageEffectParams...
			DamageEffectParams.DeathImpulse = DeathImpulse; // Death impulse vector
			DamageEffectParams.TargetASC = TargetASC; // Target ability system component
			UAuroraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}
		
		Destroy();
	}
	
	else bHit = true;
	
}

void AAuroraProjectile::OnProjectileActivate()
{
	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Green, FString::Printf(
		TEXT("[%hs]: Activated after %f. bHoming = %hhd"),
		__FUNCTION__,
		ProjectileBehaviorParams.ActivationSpan,
		ProjectileBehaviorParams.bHoming
		));
	
}



