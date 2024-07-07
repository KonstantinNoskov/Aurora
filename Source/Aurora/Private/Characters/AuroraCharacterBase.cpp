#include "Aurora/Public/Characters/AuroraCharacterBase.h"

#include "AbilitySystemComponent.h"


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
	//
}

UAbilitySystemComponent* AAuroraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

#pragma endregion

#pragma region PRIMARY ATTRIBUTES

void AAuroraCharacterBase::InitializePrimaryAttribute() const
{
	if (AbilitySystemComponent && DefaultPrimaryAttributes)
	{
		const FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
		const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultPrimaryAttributes, 1.f, ContextHandle);
		AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), AbilitySystemComponent);
	}
}

#pragma endregion


