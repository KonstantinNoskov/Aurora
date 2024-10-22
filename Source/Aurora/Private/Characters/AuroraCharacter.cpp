#include "Characters/AuroraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AuroraGameplayTags.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/AuroraAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Camera/CameraComponent.h"
#include "Controllers/PlayerControllers/AuroraPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/AuroraPlayerState.h"
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

	// Orientation 
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// Character defaults
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
	AddCharacterAbilities();
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
	// WARNING: It should be called after ASC initialized.
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
			//AuroraHUD->InitAttributeMenu(AuroraPlayerController, AuroraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}

int32 AAuroraCharacter::GetPlayerLevel_Implementation()
{
	AAuroraPlayerState* AuroraPlayerState = GetPlayerState<AAuroraPlayerState>();
	checkf(AuroraPlayerState, TEXT("AuroraCharacter.cpp - Aurora playerstate is null!"))

	return AuroraPlayerState->GetPlayerLevel();
}

void AAuroraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
}

#pragma endregion


