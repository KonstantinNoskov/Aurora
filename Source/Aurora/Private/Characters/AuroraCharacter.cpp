﻿#include "Characters/AuroraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AuroraGameplayTags.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/AuroraAbilitySystemComponent.h"
#include "AbilitySystem/AuroraAbilitySystemLibrary.h"
#include "AbilitySystem/AuroraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Camera/CameraComponent.h"
#include "Controllers/PlayerControllers/AuroraPlayerController.h"
#include "Game/AuroraGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameModes/AuroraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuroraPlayerState.h"
#include "Saves/LoadScreenSaveGame.h"
#include "UI/HUD/AuroraHUD.h"

AAuroraCharacter::AAuroraCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Camera
	AuroraSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("AuroraSpringArm");
	AuroraSpringArmComponent->SetupAttachment(GetRootComponent());
	AuroraSpringArmComponent->TargetArmLength = 400.f;
	AuroraSpringArmComponent->SetUsingAbsoluteRotation(true);
	AuroraSpringArmComponent->bDoCollisionTest = false;
	AuroraSpringArmComponent->bUsePawnControlRotation = true;
	
	AuroraCameraComponent = CreateDefaultSubobject<UCameraComponent>("AuroraCamera");
	AuroraCameraComponent->SetupAttachment(AuroraSpringArmComponent, USpringArmComponent::SocketName);
	AuroraCameraComponent->bUsePawnControlRotation = false;

	// Character Movement 
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// Character Class Default Data
	CharacterClass = ECharacterClass::Elementalist;

	// VFX
	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;
}

void AAuroraCharacter::BeginPlay()
{
	Super::BeginPlay();
}
void AAuroraCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void AAuroraCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
void AAuroraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
	LoadProgress();

	if (AAuroraGameModeBase* AuroraGameMode = Cast<AAuroraGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		AuroraGameMode->LoadWorldState(GetWorld());
	}
}

// Save/Load Progress
void AAuroraCharacter::SaveProgress_Implementation(const FName& CheckPointTag)
{
	AAuroraGameModeBase* AuroraGameMode = Cast<AAuroraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!AuroraGameMode) return;

	UAuroraGameInstance* AuroraGameInstance = Cast<UAuroraGameInstance>(AuroraGameMode->GetGameInstance());
	if (AuroraGameInstance)
	{
		ULoadScreenSaveGame* SaveData = AuroraGameMode->RetrieveInGameSaveData();
		if (!SaveData) return;
		
		SaveData->PlayerStartTag = CheckPointTag;
		
		// Save Player State Data
		if (AAuroraPlayerState* AuroraPlayerState = Cast<AAuroraPlayerState>(GetPlayerState()))
		{
			SaveData->PlayerLevel		= AuroraPlayerState->GetPlayerLevel();
			SaveData->XP				= AuroraPlayerState->GetXP();
			SaveData->AttributePoints	= AuroraPlayerState->GetAttributePoints();
			SaveData->SpellPoints		= AuroraPlayerState->GetSpellPoints();
		}

		// Save Attributes Data
		SaveData->Strength		= UAuroraAttributeSet::GetStrengthAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Intelligence	= UAuroraAttributeSet::GetIntelligenceAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Resilience	= UAuroraAttributeSet::GetResilienceAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Vigor			= UAuroraAttributeSet::GetVigorAttribute().GetNumericValue(GetAttributeSet());
		
		SaveData->bFirstTimeLoadIn = false;
		
		// Save Abilities Data
		if (!HasAuthority()) return;

		UAuroraAbilitySystemComponent* AuroraASC = Cast<UAuroraAbilitySystemComponent>(AbilitySystemComponent);
		FForEachAbilitySignature SaveAbilityDelegate;
		SaveData->SavedAbilities.Empty();
		
		SaveAbilityDelegate.BindLambda([this, AuroraASC, &SaveData](const FGameplayAbilitySpec& AbilitySpec)
		{
			const FGameplayTag AbilityTag = AuroraASC->GetAbilityTagFromSpec(AbilitySpec);
			UAbilityInfo* AbilityInfo = UAuroraAbilitySystemLibrary::GetAbilityInfo(this);
			FAuroraAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
			
			FSavedAbility SavedAbility;
			SavedAbility.GameplayAbilityClass = Info.Ability;
			SavedAbility.AbilityLevel = AbilitySpec.Level;
			SavedAbility.AbilitySlotTag = AuroraASC->GetSlotFromAbilityTag(AbilityTag);
			SavedAbility.AbilityStatusTag = AuroraASC->GetAbilityStatusFromTag(AbilityTag);
			SavedAbility.GameplayAbilityTag = AbilityTag;
			SavedAbility.AbilityTypeTag = Info.AbilityTypeTag;

			SaveData->SavedAbilities.AddUnique(SavedAbility);
		});

		AuroraASC->ForEachAbility(SaveAbilityDelegate);
		AuroraGameMode->SaveInGameProgressData(SaveData);
	}
}
void AAuroraCharacter::LoadProgress()
{
	AAuroraGameModeBase* AuroraGameMode = Cast<AAuroraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!AuroraGameMode) return;

	UAuroraGameInstance* AuroraGameInstance = Cast<UAuroraGameInstance>(AuroraGameMode->GetGameInstance());
	if (AuroraGameInstance)
	{
		ULoadScreenSaveGame* SaveData = AuroraGameMode->RetrieveInGameSaveData();
		if (!SaveData) return;
		
		if (SaveData->bFirstTimeLoadIn)
		{
			InitializeDefaultAttributes();
			AddCharacterAbilities();
		}
		else
		{
			// Load Abilities Data
			if (UAuroraAbilitySystemComponent* AuroraASC = Cast<UAuroraAbilitySystemComponent>(AbilitySystemComponent))
			{
				
				AuroraASC->AddCharacterAbilitiesFromSaveData(SaveData);
			}
			
			// Load Player State Data
			if (AAuroraPlayerState* AuroraPlayerState = Cast<AAuroraPlayerState>(GetPlayerState()))
			{
				AuroraPlayerState->SetLevel(SaveData->PlayerLevel);
				AuroraPlayerState->SetXP(SaveData->XP);
				AuroraPlayerState->SetAttributePoints(SaveData->AttributePoints);
				AuroraPlayerState->SetSpellPoints(SaveData->SpellPoints);
			}
			
			UAuroraAbilitySystemLibrary::InitializeDefaultAttributesFromSaveData(this, AbilitySystemComponent, SaveData);
		}
	}
}

#pragma region Player Interface overriden functions

// XP
void AAuroraCharacter::AddToXP_Implementation(int32 InXP)
{
	AAuroraPlayerState* AuroraPlayerState = GetPlayerState<AAuroraPlayerState>();
	checkf(AuroraPlayerState, TEXT("AuroraCharacter.cpp - Aurora playerstate is null!"))

	AuroraPlayerState->AddToXP(InXP);
}
int32 AAuroraCharacter::GetXP_Implementation() const
{
	AAuroraPlayerState* AuroraPlayerState = GetPlayerState<AAuroraPlayerState>();
	checkf(AuroraPlayerState, TEXT("[%hs] - Aurora playerstate is null!"), __FUNCTION__)

	return  AuroraPlayerState->GetXP();
}

// Level
void AAuroraCharacter::LevelUp_Implementation()
{
	MulticastLevelUpParticles();
}
int32 AAuroraCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
	AAuroraPlayerState* AuroraPlayerState = GetPlayerState<AAuroraPlayerState>();
	checkf(AuroraPlayerState, TEXT("[%hs] - Aurora playerstate is null!"), __FUNCTION__)

	return  AuroraPlayerState->LevelUpInfo->FindLevelForXP(InXP);
}
void AAuroraCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	AAuroraPlayerState* AuroraPlayerState = GetPlayerState<AAuroraPlayerState>();
	checkf(AuroraPlayerState, TEXT("[%hs] - Aurora playerstate is null!"), __FUNCTION__)
	AuroraPlayerState->AddToLevel(InPlayerLevel);

	if (UAuroraAbilitySystemComponent* AuroraASC = Cast<UAuroraAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		AuroraASC->UpdateAbilityStatus(AuroraPlayerState->GetPlayerLevel());
	}
}
void AAuroraCharacter::MulticastLevelUpParticles_Implementation() const
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		const FVector CameraLocation = AuroraCameraComponent->GetComponentLocation();
		const FVector NiagaraSystemLocation = LevelUpNiagaraComponent->GetComponentLocation();

		const FRotator ToCameraRotation = (CameraLocation - NiagaraSystemLocation).Rotation();

		LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);
		LevelUpNiagaraComponent->Activate(true);
	}
}

// Attribute Points
int32 AAuroraCharacter::GetAttributePointsReward_Implementation(int32 Level) const
{
	AAuroraPlayerState* AuroraPlayerState = GetPlayerState<AAuroraPlayerState>();
	checkf(AuroraPlayerState, TEXT("[%hs] - Aurora playerstate is null!"), __FUNCTION__)

	return AuroraPlayerState->LevelUpInfo->LevelUpInformation[Level].AttributePointReward;
}
int32 AAuroraCharacter::GetAttributePoints_Implementation() const
{
	AAuroraPlayerState* AuroraPlayerState = GetPlayerState<AAuroraPlayerState>();
	checkf(AuroraPlayerState, TEXT("[%hs] - Aurora playerstate is null!"), __FUNCTION__)

	return AuroraPlayerState->GetAttributePoints();
}
void AAuroraCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	AAuroraPlayerState* AuroraPlayerState = GetPlayerState<AAuroraPlayerState>();
	checkf(AuroraPlayerState, TEXT("[%hs] - Aurora playerstate is null!"), __FUNCTION__)
	
	AuroraPlayerState->AddToAttributePoints(InAttributePoints);
}

// Spell Points
int32 AAuroraCharacter::GetSpellPointsReward_Implementation(int32 Level) const
{
	AAuroraPlayerState* AuroraPlayerState = GetPlayerState<AAuroraPlayerState>();
	checkf(AuroraPlayerState, TEXT("[%hs] - Aurora playerstate is null!"), __FUNCTION__)

	return AuroraPlayerState->LevelUpInfo->LevelUpInformation[Level].SpellPointReward;
}
int32 AAuroraCharacter::GetSpellPoints_Implementation() const
{
	AAuroraPlayerState* AuroraPlayerState = GetPlayerState<AAuroraPlayerState>();
	checkf(AuroraPlayerState, TEXT("[%hs] - Aurora playerstate is null!"), __FUNCTION__)

	return AuroraPlayerState->GetSpellPoints();
}
void AAuroraCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	AAuroraPlayerState* AuroraPlayerState = GetPlayerState<AAuroraPlayerState>();
	checkf(AuroraPlayerState, TEXT("[%hs] - Aurora playerstate is null!"), __FUNCTION__)
	
	AuroraPlayerState->AddToSpellPoints(InSpellPoints);
}

// Magic Circle
AMagicCircle* AAuroraCharacter::ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial, float InMagicCircleRadius)
{
	if (AAuroraPlayerController* AuroraPlayerController = Cast<AAuroraPlayerController>(GetController()))
	{
		AMagicCircle* MagicCircle = AuroraPlayerController->ShowMagicCircle(DecalMaterial, InMagicCircleRadius);
		AuroraPlayerController->bShowMouseCursor = false;

		return MagicCircle;
	}
	
	return nullptr;
}
void AAuroraCharacter::HideMagicCircle_Implementation()
{
	if (AAuroraPlayerController* AuroraPlayerController = Cast<AAuroraPlayerController>(GetController()))
	{
		AuroraPlayerController->HideMagicCircle();
		AuroraPlayerController->bShowMouseCursor = true;
	}
}

#pragma endregion
#pragma region ABILITY SYSTEM

void AAuroraCharacter::InitAbilityActorInfo()
{
	// Aurora player state valid check
	AAuroraPlayerState* AuroraPlayerState = GetPlayerState<AAuroraPlayerState>();
	
	checkf(AuroraPlayerState, TEXT("AuroraPlayerState not set. Checkout for Auto Posses Player paramater in Pawn settings."));
	//if (!AuroraPlayerState) return;	
	
	// Set ability actor info
	AuroraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuroraPlayerState, this);
	
	Cast<UAuroraAbilitySystemComponent>(AuroraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	
	// Set AbilitySystemComponent
	AbilitySystemComponent = AuroraPlayerState->GetAbilitySystemComponent();

	// Set attribute set
	AttributeSet = AuroraPlayerState->GetAttributeSet();

	// On AbilitySystemComponent assigned.
	// It should be called after ASC initialized.
	OnASCRegistered.Broadcast(AbilitySystemComponent);

	AbilitySystemComponent->RegisterGameplayTagEvent(FAuroraGameplayTags::Get().Debuff_Shock, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAuroraCharacter::StunTagChanged);

	// If AuroraHUD initialized, assign widget controllers to their widgets. 
	AssignWidgetControllers(AuroraPlayerState);
	
	// Set default attributes
	InitializeDefaultAttributes();
}

void AAuroraCharacter::AssignWidgetControllers(AAuroraPlayerState* AuroraPlayerState)
{
	
	if (AAuroraPlayerController* AuroraPlayerController = Cast<AAuroraPlayerController>(GetController()))
	{
		if (AAuroraHUD* AuroraHUD = Cast<AAuroraHUD>(AuroraPlayerController->GetHUD()))
		{
			AuroraHUD->InitOverlay(AuroraPlayerController, AuroraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}

int32 AAuroraCharacter::GetPlayerLevel_Implementation()
{
	AAuroraPlayerState* AuroraPlayerState = GetPlayerState<AAuroraPlayerState>();
	checkf(AuroraPlayerState, TEXT("AuroraCharacter.cpp - Aurora playerstate is null!"))

	return AuroraPlayerState->GetPlayerLevel();
}

void AAuroraCharacter::Die(const FVector& InDeathImpulse)
{
	Super::Die(InDeathImpulse);

	FTimerDelegate DeathTimerDelegate;
	DeathTimerDelegate.BindLambda([this]()
	{
		AAuroraGameModeBase* AuroraGM = Cast<AAuroraGameModeBase>(UGameplayStatics::GetGameMode(this));
		if (AuroraGM)
		{
			AuroraGM->PlayerDied(this);
		}
	});

	GetWorldTimerManager().SetTimer(DeathTimer, DeathTimerDelegate, DeathTime, false);
	AuroraCameraComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	GetMovementComponent()->Deactivate();
}

void AAuroraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
}

#pragma endregion


