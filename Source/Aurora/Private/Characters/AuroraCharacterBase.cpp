#include "Aurora/Public/Characters/AuroraCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuroraAbilitySystemComponent.h"
#include "Aurora/Aurora.h"
#include "Components/CapsuleComponent.h"
#include "Debug/DebugMacros.h"


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
}

void AAuroraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
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
}

#pragma endregion

#pragma endregion

#pragma region COMBAT

FVector AAuroraCharacterBase::GetCombatSocketLocation()
{
	check(Weapon)
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}

void AAuroraCharacterBase::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath();
}

void AAuroraCharacterBase::MulticastHandleDeath_Implementation()
{
	// Enable "ragdoll mode" 
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	
	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);

	// Start to dissolve the died character and his weapon 
	Dissolve();
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