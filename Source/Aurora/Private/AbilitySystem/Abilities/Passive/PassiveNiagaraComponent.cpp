#include "AbilitySystem/Abilities/Passive/PassiveNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuroraAbilitySystemComponent.h"
#include "Interfaces/Interaction/CombatInterface.h"


UPassiveNiagaraComponent::UPassiveNiagaraComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}


void UPassiveNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UAuroraAbilitySystemComponent* AuroraASC = Cast<UAuroraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
	{
		AuroraASC->OnPassiveAbilityActivated.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveActivate);
	}
	else if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner()))
	{
		CombatInterface->GetOnAscRegisteredDelegate().AddLambda([this](UAbilitySystemComponent* ASC)
		{
			if (UAuroraAbilitySystemComponent* AuroraASC = Cast<UAuroraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
			{
				AuroraASC->OnPassiveAbilityActivated.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveActivate);
			}
		});
	}
}

void UPassiveNiagaraComponent::OnPassiveActivate(const FGameplayTag& InAbilityTag, bool bActivate)
{
	if (InAbilityTag.MatchesTagExact(PassiveSpellTag))
	{
		if (bActivate && !IsActive())
		{
			Activate();
		}
		else
		{
			Deactivate();
		}
	}
}



