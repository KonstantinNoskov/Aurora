#include "Aurora/Public/Characters/AuroraCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuroraAbilitySystemComponent.h"
#include "Debug/DebugMacros.h"


AAuroraCharacterBase::AAuroraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
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

#pragma endregion