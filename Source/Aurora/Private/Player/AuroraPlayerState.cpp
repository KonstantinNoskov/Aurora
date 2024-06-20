#include "Player/AuroraPlayerState.h"

// Ability System
#include "AbilitySystem/AuroraAbilitySystemComponent.h"
#include "AbilitySystem/AuroraAttributeSet.h"
#include "Characters/AuroraCharacterBase.h"

AAuroraPlayerState::AAuroraPlayerState()
{
	// Ability system component
	AbilitySystemComponent = CreateDefaultSubobject<UAuroraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// Attribute set
	AttributeSet = CreateDefaultSubobject<UAuroraAttributeSet>("AttributeSet");
	
	NetUpdateFrequency = 100.f;
}

#pragma region ABILITY SYSTEM

UAbilitySystemComponent* AAuroraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuroraPlayerState::BeginPlay()
{
	Super::BeginPlay();

	const UAuroraAttributeSet* AuroraAttributeSet = Cast<UAuroraAttributeSet>(AbilitySystemComponent->GetAttributeSet(UAuroraAttributeSet::StaticClass()));
	UAuroraAttributeSet* MutableAuroraAttributeSet = const_cast<UAuroraAttributeSet*>(AuroraAttributeSet);
	MutableAuroraAttributeSet->SetHealth(10.f);
}

#pragma endregion}
