
#include "AbilitySystem/AuroraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuroraGameplayTags.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Interfaces/Interaction/CombatInterface.h"
#include "Net/UnrealNetwork.h"

UAuroraAttributeSet::UAuroraAttributeSet()
{
	const FAuroraGameplayTags& GameplayTags = FAuroraGameplayTags::Get();

#pragma region Attribute Tags Mapping

	// Primary
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Strength,					GetStrengthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Intelligence,				GetIntelligenceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Resilience,				GetResilienceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Vigor,						GetVigorAttribute);

	// Secondary
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Armor,					GetArmorAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ArmorPenetration,		GetArmorPenetrationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_BlockChance,				GetBlockChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_HealthRegeneration,		GetHealthRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ManaRegeneration,		GetManaRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth,				GetMaxHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxMana,					GetMaxManaAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitChance,		GetCriticalHitChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitDamage,		GetCriticalHitDamageAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitResistance,	GetCriticalHitResistanceAttribute);

	// Vital
	/*TagsToAttributes.Add(GameplayTags.Attributes_Vital_Health,					GetHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Vital_Mana,						GetManaAttribute);*/

#pragma endregion
}

void UAuroraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// Primary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, Strength,				COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, Intelligence,			COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, Resilience,				COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, Vigor,					COND_None, REPNOTIFY_Always);

	// Secondary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, Armor,					COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, ArmorPenetration,		COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, BlockChance,			COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, CriticalHitChance,		COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, CriticalHitDamage,		COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, CriticalHitResistance,	COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, HealthRegeneration,		COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, ManaRegeneration,		COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, MaxHealth,				COND_None, REPNOTIFY_Always);	
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, MaxMana,				COND_None, REPNOTIFY_Always);

	// Vital Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, Health,					COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, Mana,					COND_None, REPNOTIFY_Always);
}
void UAuroraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}
void UAuroraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data,Props);

	// Health Changed
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0 , GetMaxHealth()));
	}

	// Mana changed
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0 , GetMaxMana()));
	}

	// Damage taken
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetIncomingDamage();

		// Reset incoming damage
		SetIncomingDamage(0.f);

		// Apply incoming damage if > 0
		if (LocalIncomingDamage > 0.f)
		{
			// New Health = Old Health - Incoming damage
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

			// Check if taken damage is fatal
			const bool bFatal = NewHealth <= 0.f;

			// If damage is fatal, check if avatar has a combat interface
			// and if it does, call die function from it.
			if (bFatal)
			{
				if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor))
				{
					CombatInterface->Die();
				}
			}
			// If damage not fatal activate HitReact ability (which is basically hit react animation montage)
			else 
			{
				// TryActivateAbilitiesByTag function receives only TagContainer as an argument
				// so we create TagContainer and add HitReact Tag to it.
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FAuroraGameplayTags::Get().Effects_HitReact);
				
				Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}
		}
	}
	
}
void UAuroraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	// Source = causer of the effect, Target = target of the effect (owner of this AS)
	
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		
		if (!IsValid(Props.SourceController) && !IsValid(Props.SourceAvatarActor))
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}

		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

#pragma region VITAL ATTRIBUTES

void UAuroraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, Health, OldHealth);
}
void UAuroraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, Mana, OldMana);
}

#pragma endregion
#pragma region PRIMARY ATTRIBUTES

void UAuroraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, Strength, OldStrength);
}
void UAuroraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, Intelligence, OldIntelligence);
}
void UAuroraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, Resilience, OldResilience);
}
void UAuroraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, Vigor, OldVigor);
}

#pragma endregion
#pragma region SECONDARY ATTRIBUTES

void UAuroraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, MaxHealth, OldMaxHealth);
}
void UAuroraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const 
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, MaxMana, OldMaxMana); 
}
void UAuroraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, Armor, OldArmor);
}
void UAuroraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, ArmorPenetration, OldArmorPenetration);
}
void UAuroraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, BlockChance, OldBlockChance);
}
void UAuroraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, CriticalHitChance, OldCriticalHitChance);
}
void UAuroraAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}
void UAuroraAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}
void UAuroraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, HealthRegeneration, OldHealthRegeneration);
}
void UAuroraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, ManaRegeneration, OldManaRegeneration);
}

#pragma endregion

