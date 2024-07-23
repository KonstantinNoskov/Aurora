#include "Characters/AuroraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuroraAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Controllers/PlayerControllers/AuroraPlayerController.h"
#include "Debug/DebugMacros.h"
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

#pragma region ABILITY SYSTEM

void AAuroraCharacter::InitAbilityActorInfo()
{
	
	// Aurora player state valid check
	AAuroraPlayerState* AuroraPlayerState = GetPlayerState<AAuroraPlayerState>();
	
	//checkf(AuroraPlayerState, TEXT("AuroraPlayerState not set. Checkout for Auto Posses Player paramater in Pawn settings."));
	if (!AuroraPlayerState) return;
	
	// Set ability actor info
	AuroraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuroraPlayerState, this);
	
	Cast<UAuroraAbilitySystemComponent>(AuroraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	
	// Set abilitySystemComponent
	AbilitySystemComponent = AuroraPlayerState->GetAbilitySystemComponent();

	// Set attribute set
	AttributeSet = AuroraPlayerState->GetAttributeSet();
	
	if (AAuroraPlayerController* AuroraPlayerController = Cast<AAuroraPlayerController>(GetController()))
	{
		if (AAuroraHUD* AuroraHUD = Cast<AAuroraHUD>(AuroraPlayerController->GetHUD()))
		{
			AuroraHUD->InitOverlay(AuroraPlayerController, AuroraPlayerState, AbilitySystemComponent, AttributeSet);
			AuroraHUD->InitAttributeMenu(AuroraPlayerController, AuroraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}



	// Set default attributes
	InitializeDefaultAttributes();
}

int32 AAuroraCharacter::GetPlayerLevel()
{
	const AAuroraPlayerState* AuroraPlayerState = GetPlayerState<AAuroraPlayerState>();
	checkf(AuroraPlayerState, TEXT("AuroraCharacter.cpp - Aurora playerstate is null!"))

	return  AuroraPlayerState->GetPlayerLevel();
}

void AAuroraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
}

#pragma endregion


