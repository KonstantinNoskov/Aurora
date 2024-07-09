#include "Player/AuroraPlayerState.h"

// Ability System
#include "AbilitySystem/AuroraAbilitySystemComponent.h"
#include "AbilitySystem/AuroraAttributeSet.h"
#include "Net/UnrealNetwork.h"

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


void AAuroraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAuroraPlayerState, Level)	
}

void AAuroraPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
}

#pragma region ABILITY SYSTEM


UAbilitySystemComponent* AAuroraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent; 
}

void AAuroraPlayerState::OnRep_Level(int32 OldLevel)
{
	
}
#pragma endregion
