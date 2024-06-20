

#include "AbilitySystem/AuroraAttributeSet.h"

#include "Net/UnrealNetwork.h"

UAuroraAttributeSet::UAuroraAttributeSet()
{
	InitHealth(100.f);
	InitMaxHealth(100.f);
	InitMana(50.f);
	InitMaxMana(50.f);
}

void UAuroraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Health
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);

	// Mana
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}


#pragma region HEALTH

void UAuroraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, Health, OldHealth);
}
void UAuroraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, MaxHealth, OldMaxHealth);
}

#pragma endregion

#pragma region MANA

void UAuroraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, Mana, OldMana);
}

void UAuroraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, MaxMana, OldMaxMana);
}

#pragma endregion
