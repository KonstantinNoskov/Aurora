#include "Controllers/PlayerControllers/AuroraPlayerController.h"

// Enhanced Input
#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/AuroraAbilitySystemComponent.h"
#include "Debug/DebugMacros.h"
#include "Input/AuroraInputComponent.h"
#include "Interfaces/Interaction/TargetInterface.h"

AAuroraPlayerController::AAuroraPlayerController()
{
	bReplicates = true;
}

void AAuroraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Aurora mapping context valid check
	check(AuroraMappingContext)

	// Set EnhancedInput
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(Subsystem)
	{
		// Set MappingContext
		Subsystem->AddMappingContext(AuroraMappingContext, 0);	
	}
	
	// Mouse Cursor
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	// Input Mode
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuroraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

#pragma region INPUT

void AAuroraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuroraInputComponent* AuroraInputComponent = CastChecked<UAuroraInputComponent>(InputComponent);

	// Move
	AuroraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuroraPlayerController::Move);

	AuroraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}


void AAuroraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection,	InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection,	InputAxisVector.X);
	}
}

#pragma endregion

#pragma region ABILITIES

UAuroraAbilitySystemComponent* AAuroraPlayerController::GetASC()
{
	if (!AuroraAbilitySystemComponent)
	{
		AuroraAbilitySystemComponent = Cast<UAuroraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));  
	}

	return AuroraAbilitySystemComponent;
}

void AAuroraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	//DEBUG_MESSAGE_STRING_COLOR_KEY(InputTag.ToString(), FColor::Green, 1)
}

void AAuroraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!GetASC()) return;
	
	GetASC()->AbilityInputTagReleased(InputTag);
}

void AAuroraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!GetASC()) return;
	
	GetASC()->AbilityInputTagHeld(InputTag);
}

#pragma endregion

void AAuroraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	
	if (CursorHit.bBlockingHit)
	{
		LastActor = ThisActor;
		ThisActor = CursorHit.GetActor();
		
		/*	We have several cases:
		 *	
		 *  1. LastActor is null && ThisActor is null. (Hovering cursor over something else than enemies)
		 *		- Do nothing
		 *
		 *	2. LastActor is Target && ThisActor is Target and they are the same target. (Keep hovering cursor over the same enemy)
		 *		- Do nothing
		 *	
		 *  3. LastActor is null && ThisActor is Target. (Start hovering cursor over the enemy)
		 *		- HighLight this Target.
		 *		
		 *  4. LastActor is Target && ThisActor is null. (Stop hovering cursor over the enemy)
		 *		- Unhighlight last Target.
		 *		
		 *  5. LastActor is Target && ThisActor is Target, but LastActor != ThisActor. (Different enemies)
		 *		- Unhighlight LastActor and Highlight ThisActor.
		 */

		if (LastActor == ThisActor) return;							// Case #1 and Case #2
		if (!LastActor && ThisActor) ThisActor->HighLightActor();	// Case #3
		if (LastActor && !ThisActor) LastActor->UnHighLightActor(); // Case #4
		if (LastActor && ThisActor && LastActor != ThisActor)		// Case #5  
		{
			LastActor->UnHighLightActor();
			ThisActor->HighLightActor();
		}
	}
}
