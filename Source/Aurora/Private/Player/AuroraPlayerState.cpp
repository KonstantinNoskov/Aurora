#include "Player/AuroraPlayerState.h"

// Ability System
#include "AbilitySystem/AuroraAbilitySystemComponent.h"
#include "AbilitySystem/AuroraAttributeSet.h"
#include "Characters/AuroraCharacterBase.h"

AAuroraPlayerState::AAuroraPlayerState()
{
	NetUpdateFrequency = 100.f;
	
	// Ability system component
	AbilitySystemComponent = CreateDefaultSubobject<UAuroraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// Attribute set
	AttributeSet = CreateDefaultSubobject<UAuroraAttributeSet>("AttributeSet");
}

#pragma region ABILITY SYSTEM

UAbilitySystemComponent* AAuroraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuroraPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	
}

#pragma endregion}
