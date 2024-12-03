#include "Player/AuroraPlayerState.h"

// Ability System
#include "AbilitySystem/AuroraAbilitySystemComponent.h"
#include "AbilitySystem/AuroraAttributeSet.h"
#include "Net/UnrealNetwork.h"

AAuroraPlayerState::AAuroraPlayerState()
{
	// Ability system component
	AbilitySystemComponent = CreateDefaultSubobject<UAuroraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	NetUpdateFrequency = 100.f;

	// Attribute set
	AttributeSet = CreateDefaultSubobject<UAuroraAttributeSet>("AttributeSet");
}

void AAuroraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAuroraPlayerState, Level)
	DOREPLIFETIME(AAuroraPlayerState, XP)
	DOREPLIFETIME(AAuroraPlayerState, AttributePoints)
	DOREPLIFETIME(AAuroraPlayerState, SpellPoints)	
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

#pragma endregion

#pragma region XP & Level UP

void AAuroraPlayerState::SetXP(int32 NewXP)
{
	XP = NewXP;

	OnXPChanged.Broadcast(XP);
}
void AAuroraPlayerState::AddToXP(int32 AddXP)
{
	XP += AddXP;

	OnXPChanged.Broadcast(XP);
}

void AAuroraPlayerState::SetLevel(int32 NewLevel)
{
	Level = NewLevel;
	OnLevelChanged.Broadcast(Level, false);
}

void AAuroraPlayerState::AddToLevel(int32 AddLevel)
{
	Level += AddLevel;
	OnLevelChanged.Broadcast(Level, true);
}

void AAuroraPlayerState::AddToAttributePoints(int32 AddAttributePoints)
{
	AttributePoints += AddAttributePoints;
	OnAttributePointsChanged.Broadcast(AttributePoints);
}
void AAuroraPlayerState::SetAttributePoints(int32 NewAttributePoints)
{
	AttributePoints = NewAttributePoints;
	OnAttributePointsChanged.Broadcast(AttributePoints);
}

void AAuroraPlayerState::AddToSpellPoints(int32 AddSpellPoints)
{
	SpellPoints += AddSpellPoints;
	OnSpellPointsChanged.Broadcast(SpellPoints);
}
void AAuroraPlayerState::SetSpellPoints(int32 NewSpellPoints)
{
	SpellPoints = NewSpellPoints;
	OnSpellPointsChanged.Broadcast(SpellPoints);
}

void AAuroraPlayerState::OnRep_Level(int32 OldLevel) const
{
	OnLevelChanged.Broadcast(Level, true);
}
void AAuroraPlayerState::OnRep_XP(int32 OldXP) const 
{
	OnXPChanged.Broadcast(XP);
}
void AAuroraPlayerState::OnRep_AttributePoints(int32 OldAttributePoints) const
{
	OnAttributePointsChanged.Broadcast(AttributePoints);
}
void AAuroraPlayerState::OnRep_SpellPoints(int32 OldSpellPoints) const
{
	OnSpellPointsChanged.Broadcast(SpellPoints);
}

#pragma endregion
