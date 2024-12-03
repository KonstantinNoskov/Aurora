#include "Aurora/Public/Characters/AuroraCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AuroraGameplayTags.h"
#include "AbilitySystem/AuroraAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/Passive/PassiveNiagaraComponent.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "Aurora/Aurora.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


AAuroraCharacterBase::AAuroraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Capsule
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);

	// Mesh
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	// Weapon
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("SKT_WeaponHand"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Debuff Niagara Component
	DebuffNiagaraComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("Debuff Component");
	DebuffNiagaraComponent->SetupAttachment(GetRootComponent());
	DebuffNiagaraComponent->DebuffTag = FAuroraGameplayTags::Get().Debuff_Burn;

	EffectAttachComponent = CreateDefaultSubobject<USceneComponent>("EffectAttachPoint");
	EffectAttachComponent->SetupAttachment(GetRootComponent());

	HaloOfProtectionNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("HaloOfProtection");
	HaloOfProtectionNiagaraComponent->SetupAttachment(EffectAttachComponent);
	HaloOfProtectionNiagaraComponent->bAutoActivate = false;

	LifeSiphonNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("LifeSiphon");
	LifeSiphonNiagaraComponent->SetupAttachment(EffectAttachComponent);
	LifeSiphonNiagaraComponent->bAutoActivate = false;

	ManaSiphonNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("ManaSiphon");
	ManaSiphonNiagaraComponent->SetupAttachment(EffectAttachComponent);
	ManaSiphonNiagaraComponent->bAutoActivate = false;
}

void AAuroraCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AAuroraCharacterBase, bStunned)
}

void AAuroraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAuroraCharacterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	EffectAttachComponent->SetWorldRotation(FRotator::ZeroRotator);
}

void AAuroraCharacterBase::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bStunned = NewCount > 0;
				
	GetCharacterMovement()->MaxWalkSpeed = bStunned ? 0.f : BaseWalkSpeed;
}

#pragma region ABILITY SYSTEM

void AAuroraCharacterBase::InitAbilityActorInfo()
{
	// Logic is defined in child classes.
}

void AAuroraCharacterBase::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, const float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	
	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}
void AAuroraCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}
UAbilitySystemComponent* AAuroraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

#pragma region ABILITITES

void AAuroraCharacterBase::AddCharacterAbilities()
{
	UAuroraAbilitySystemComponent* AuroraASC = CastChecked<UAuroraAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;

	
	AuroraASC->AddCharacterAbilities(StartupAbilities);
	AuroraASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
	
}

#pragma endregion

#pragma endregion

#pragma region COMBAT

float AAuroraCharacterBase::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float DamageTaken = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser); 

	OnDamageDelegate.Broadcast(DamageTaken);

	return DamageTaken;
}

FVector AAuroraCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	for (TTuple<FGameplayTag, FName> Socket : CombatSockets)
	{
		if (MontageTag.MatchesTagExact(Socket.Key))
		{
			return
			Socket.Key == FAuroraGameplayTags::Get().CombatSocket_Weapon
			? Weapon->GetSocketLocation(WeaponTipSocketName) 
			: GetMesh()->GetSocketLocation(Socket.Value);
		}
	}

	return FVector();
}
const USkeletalMeshSocket* AAuroraCharacterBase::GetCombatSocketByTag_Implementation(const FGameplayTag& MontageTag)
{
	for (TTuple<FGameplayTag, FName> Socket : CombatSockets)
	{
		if (MontageTag.MatchesTagExact(Socket.Key))
		{
			return
			Socket.Key == FAuroraGameplayTags::Get().CombatSocket_Weapon
			? Weapon->GetSocketByName(WeaponTipSocketName) 
			: GetMesh()->GetSocketByName(Socket.Value);
		}
	}
	
	return nullptr;
}

FTaggedMontage AAuroraCharacterBase::GetTaggedMontageByTag_Implementation(const FGameplayTag& InMontageTag)
{
	for (FTaggedMontage TaggedMontage : AttackMontages)
	{
		if (TaggedMontage.MontageTag == InMontageTag)
		{
			return TaggedMontage;		
		}
	}
	return FTaggedMontage();
}
void AAuroraCharacterBase::SetMinionCount_Implementation(int32 NewMinionCount)
{
	MinionCount = NewMinionCount;
}
void AAuroraCharacterBase::IncrementMinionCount_Implementation(int32 Increment)
{
	MinionCount += Increment;
}
void AAuroraCharacterBase::Die(const FVector& InDeathImpulse)
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath(InDeathImpulse);
}
void AAuroraCharacterBase::MulticastHandleDeath_Implementation(const FVector& InDeathImpulse)
{
	// Play death sound
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());
	
	// Enable "ragdoll mode" 
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetMesh()->AddImpulse(InDeathImpulse, NAME_None, true);
	
	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);

	// Start to dissolve the died character and his weapon 
	Dissolve();

	// Set bDead flag;
	bDead = true;
	
	// Broadcast On Death delegate
	OnDeath.Broadcast(this);
}

#pragma endregion

#pragma region DISSOLVE EFFECT

void AAuroraCharacterBase::Dissolve()
{
	// Create Dynamic material for Character Mesh
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicMatInst);

		// Start the dissolve process
		StartDissolveTimeLine(DynamicMatInst);
	}

	// Create Dynamic material for Weapon Mesh
	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		Weapon ->SetMaterial(0, DynamicMatInst);
		
		// Start the dissolve process
		StartWeaponDissolveTimeLine(DynamicMatInst);
	}
}

#pragma endregion