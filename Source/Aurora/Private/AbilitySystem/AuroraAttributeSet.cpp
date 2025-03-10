﻿
#include "AbilitySystem/AuroraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuroraAbilityTypes.h"
#include "AuroraColors.h"
#include "AuroraGameplayTags.h"
#include "AuroraLibrary.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/AuroraAbilitySystemLibrary.h"
#include "Aurora/AuroraLogChannels.h"
#include "Controllers/PlayerControllers/AuroraPlayerController.h"
#include "GameFramework/Character.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "Interfaces/Interaction/CombatInterface.h"
#include "Interfaces/Interaction/PlayerInterface.h"
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

	// Resistances
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Fire,					GetFireResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Lightning,				GetLightningResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Arcane,					GetArcaneResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Physical,				GetPhysicalResistanceAttribute);

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

	// Resistance
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, FireResistance,			COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, LightningResistance,	COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, ArcaneResistance,		COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, PhysicalResistance,		COND_None, REPNOTIFY_Always);

	// Vital Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, Health,					COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuroraAttributeSet, Mana,					COND_None, REPNOTIFY_Always);
}
void UAuroraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::CeilToFloat(FMath::Clamp(NewValue, 0.f, GetMaxHealth()));
	}
	
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::CeilToFloat(FMath::Clamp(NewValue, 0.f, GetMaxMana()));
	}
}
void UAuroraAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute() && bTopOffHealth)
	{
		SetHealth(GetMaxHealth());
		bTopOffHealth = false;
	}

	if (Attribute == GetMaxManaAttribute() && bTopOffMana)
	{
		SetMana(GetMaxMana());
		bTopOffMana = false;
	}
}
void UAuroraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data,Props);

	// Check if Character is Dead 
	if (Props.TargetCharacter->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(Props.TargetCharacter))
	{
		return;
	}

	// Health Changed
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::CeilToFloat(FMath::Clamp(GetHealth(), 0 , GetMaxHealth())));
	}

	// Mana changed
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::CeilToFloat(FMath::Clamp(GetMana(), 0 , GetMaxMana())));
	}

	// Incoming Damage Taken
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute()) HandleIncomingDamage(Props);

	// Incoming XP Gained
	if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute()) HandelIncomingXP(Props);
}

void UAuroraAttributeSet::HandleIncomingDamage(const FEffectProperties& Props)
{
	// Store Incoming damage in local variable so we can reset Incoming damage attribute value
	const float LocalIncomingDamage = GetIncomingDamage();

	// Reset incoming damage. From now on we use LocalIncomingDamage only.
	SetIncomingDamage(0.f);

	// Apply incoming damage if > 0
	if (LocalIncomingDamage > 0.f)
	{
		
		// New Health = Old Health - Incoming damage
		const float NewHealth = GetHealth() - LocalIncomingDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

		// Handle behavior after taken damage
		TakenDamageHandle(Props, NewHealth);

		// Pass in an additional info about the damage so the widget can use it (such as whether damage was critical, blocked etc.) 
		const bool bBlock = UAuroraAbilitySystemLibrary::IsBlockedHit(Props.EffectContextHandle);
		const bool bCritical = UAuroraAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
			
		// Show damage as a floating widget
		ShowFloatingText(Props, LocalIncomingDamage, bBlock, bCritical);
		
		if (UAuroraAbilitySystemLibrary::IsDebuffSuccessfull(Props.EffectContextHandle))
		{
			Debuff(Props);
		}
	}
}
void UAuroraAttributeSet::HandelIncomingXP(const FEffectProperties& Props)
{
	const float LocalIncomingXP = GetIncomingXP();
	SetIncomingXP(0.f);
		
	UE_LOG(LogAurora, Warning, TEXT("[%hs] Incoming %f XP!"), __FUNCTION__, LocalIncomingXP)
		
	// Source Character is the owner, since GA_ListenForEvents applies GE_EventBasedEffect, adding to IncomingXP
	if (Props.SourceCharacter->Implements<UPlayerInterface>() && Props.SourceCharacter->Implements<UCombatInterface>())
	{
		const int32 CurrentLevel = ICombatInterface::Execute_GetPlayerLevel(Props.SourceCharacter);
		const int32 CurrentXP = IPlayerInterface::Execute_GetXP(Props.SourceCharacter);

		const int32 NewLevel = IPlayerInterface::Execute_FindLevelForXP(Props.SourceCharacter, CurrentXP + LocalIncomingXP);
		const int32 NumLevelUps = NewLevel - CurrentLevel;

		// Initiate Level up if level ups amount > 0 
		if (NumLevelUps > 0)
		{
			// Add levels
			IPlayerInterface::Execute_AddToPlayerLevel(Props.SourceCharacter, NumLevelUps);

			int32 AttributePointsReward = 0;
			int32 SpellPointsReward = 0;

			for (int32 i = 0; i < NumLevelUps; ++i)
			{
				SpellPointsReward += IPlayerInterface::Execute_GetSpellPointsReward(Props.SourceCharacter, CurrentLevel + i);
				AttributePointsReward += IPlayerInterface::Execute_GetAttributePointsReward(Props.SourceCharacter, CurrentLevel + i);
			}

			// Add attribute points depending on level
			IPlayerInterface::Execute_AddToAttributePoints(Props.SourceCharacter, AttributePointsReward);

			// Add spell points depending on level
			IPlayerInterface::Execute_AddToSpellPoints(Props.SourceCharacter, SpellPointsReward);
				
			// Fill up Health and Mana
			SetHealth(GetMaxHealth());
			SetMana(GetMaxMana());
				
			bTopOffHealth = true;
			bTopOffMana = true;

			// Call avatar level up handle
			IPlayerInterface::Execute_LevelUp(Props.SourceCharacter);
		}
			
		IPlayerInterface::Execute_AddToXP(Props.SourceCharacter, LocalIncomingXP);
	}
}
void UAuroraAttributeSet::TakenDamageHandle(const FEffectProperties& Props, const float NewHealth)
{
	// Check if taken damage is fatal
	const bool bFatal = NewHealth <= 0.f;

	// If damage is fatal, check if avatar has a combat interface
	// and if it does, call die function from it.
	if (bFatal)
	{
		// Apply Death impulse
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor))
		{
			const FVector Impulse = UAuroraAbilitySystemLibrary::GetDeathImpulse(Props.EffectContextHandle);
			CombatInterface->Die(Impulse);
		}

		SendXPEvent(Props);
	}
	// If damage not fatal, activate HitReact ability (which is basically hit react animation montage)
	else 
	{
		// Local TagContainer
		FGameplayTagContainer TagContainer;
		TagContainer.AddTag(FAuroraGameplayTags::Get().Effects_HitReact);
				
		Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
	}
}
void UAuroraAttributeSet::SendXPEvent(const FEffectProperties& Props)
{
	if (Props.TargetCharacter->Implements<UCombatInterface>())
	{
		ECharacterClass TargetClass = ICombatInterface::Execute_GetCharacterClass(Props.TargetCharacter);
		int32 TargetLevel = ICombatInterface::Execute_GetPlayerLevel(Props.TargetCharacter);
		const int32 XPReward = UAuroraAbilitySystemLibrary::GetXPRewardForClassAndLevel(this, TargetClass, TargetLevel);

		const FAuroraGameplayTags& GameplayTags = FAuroraGameplayTags::Get();

		FGameplayEventData Payload;
		Payload.EventTag = GameplayTags.Attributes_Meta_IncomingXP;
		Payload.EventMagnitude = XPReward;
		
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter, GameplayTags.Attributes_Meta_IncomingXP, Payload);
	}
}
void UAuroraAttributeSet::Debuff(const FEffectProperties& InProps)
{
	const FAuroraGameplayTags& GameplayTags = FAuroraGameplayTags::Get();
	FGameplayEffectContextHandle EffectContext = InProps.SourceASC->MakeEffectContext();
	EffectContext.AddSourceObject(InProps.SourceAvatarActor);
	
	const FGameplayTag DamageTypeTag = UAuroraAbilitySystemLibrary::GetDamageType(InProps.EffectContextHandle);
	const float DebuffDamage = UAuroraAbilitySystemLibrary::GetDebuffDamage(InProps.EffectContextHandle);
	const float DebuffDuration = UAuroraAbilitySystemLibrary::GetDebuffDuration(InProps.EffectContextHandle);
	const float DebuffFrequency = UAuroraAbilitySystemLibrary::GetDebuffFrequency(InProps.EffectContextHandle);

	// Create a new Gameplay Effect
	FString DebuffName = FString::Printf(TEXT("DynamicDebuff_%s"), *DamageTypeTag.ToString());
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(DebuffName));

	// Set Duration Policy, Period and Duration Magnitude
	Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	Effect->Period = DebuffFrequency;
	Effect->bExecutePeriodicEffectOnApplication = false;
	Effect->DurationMagnitude = FScalableFloat(DebuffDuration);

	// Add Granted tags (UE 5.4) InheritedTags == GrantedTags
	FInheritedTagContainer TagContainer = FInheritedTagContainer();
	const FGameplayTag DebuffTag = GameplayTags.DamageTypesToDebuffs[DamageTypeTag];
	
	UTargetTagsGameplayEffectComponent& EffectComponent = Effect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	TagContainer.Added.AddTag(DebuffTag);
	
	
	if (DebuffTag.MatchesTagExact(GameplayTags.Debuff_Shock))
	{
		TagContainer.AddTag(GameplayTags.Player_Block_CursorTrace);
		TagContainer.AddTag(GameplayTags.Player_Block_InputHeld);
		TagContainer.AddTag(GameplayTags.Player_Block_InputPressed);
		TagContainer.AddTag(GameplayTags.Player_Block_InputReleased);
	}

	EffectComponent.SetAndApplyTargetTagChanges(TagContainer);

	// Stacks
	Effect->StackingType = EGameplayEffectStackingType::AggregateByTarget;
	Effect->StackLimitCount = 1;

	// Modifiers 
	const int32 Index = Effect->Modifiers.Num();
	Effect->Modifiers.Add(FGameplayModifierInfo());
	FGameplayModifierInfo& ModifierInfo = Effect->Modifiers[Index];
	ModifierInfo.ModifierMagnitude = FScalableFloat(DebuffDamage);
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.Attribute = GetIncomingDamageAttribute();

	// Apply gameplay effect
	if (FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect, EffectContext, 1.f))
	{
		FAuroraGameplayEffectContext* AuroraContext = static_cast<FAuroraGameplayEffectContext*>(MutableSpec->GetContext().Get());

		TSharedPtr<FGameplayTag> DebuffDamageTypeTag = MakeShareable(new FGameplayTag(DamageTypeTag)); 
		AuroraContext->SetDamageType(DebuffDamageTypeTag);
		
		InProps.TargetASC->ApplyGameplayEffectSpecToSelf(*MutableSpec);
	}
}

void UAuroraAttributeSet::ShowFloatingText(const FEffectProperties& Props, const float Damage, bool bBlockedHit, bool bCriticalHit) const
{
	if (Props.SourceCharacter != Props.TargetCharacter)
	{
		if (AAuroraPlayerController* PC = Cast<AAuroraPlayerController>(Props.SourceCharacter->Controller))
		{

			// Get Damage Type Tag to pass it in to the Damage Floating Text
			const FGameplayTag DamageTypeTag = UAuroraAbilitySystemLibrary::GetDamageType(Props.EffectContextHandle);
			
			PC->ShowDamageNumber(Damage, Props.TargetCharacter, bBlockedHit, bCriticalHit, DamageTypeTag);
		}
	}
}
void UAuroraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	// Source = causer of the effect, Target = target of the effect (owner of this AS)
	
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	// Avatar Actor Valid Check
	bool bAvatarActorValid =
		IsValid(Props.SourceASC) &&									// SourceASC is valid
		Props.SourceASC->AbilityActorInfo.IsValid() &&				// Ability Actor Info is valid
		Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid();	// Avatar Actor is valid
	
	if (bAvatarActorValid)
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		
		if (!IsValid(Props.SourceController) && IsValid(Props.SourceAvatarActor))
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

#pragma region Replication Callbacks

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

void UAuroraAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, FireResistance, OldFireResistance);
}
void UAuroraAttributeSet::OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, LightningResistance, OldLightningResistance);
}
void UAuroraAttributeSet::OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, ArcaneResistance, OldArcaneResistance);
}
void UAuroraAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuroraAttributeSet, PhysicalResistance, OldPhysicalResistance);
}

#pragma endregion

#pragma endregion


