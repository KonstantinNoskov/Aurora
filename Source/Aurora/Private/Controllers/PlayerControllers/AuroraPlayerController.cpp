#include "Controllers/PlayerControllers/AuroraPlayerController.h"

// Enhanced Input
#include "EnhancedInputSubsystems.h"
#include "Input/AuroraInputComponent.h"

// Movement Navigation
#include "NavigationSystem.h"
#include "Components/SplineComponent.h"

// Gameplay Abilities
#include "AbilitySystem/AuroraAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuroraGameplayTags.h"

// Interaction
#include "Interfaces/Interaction/TargetInterface.h"

// Debug
#include "NavigationPath.h"
#include "Debug/DebugMacros.h"

AAuroraPlayerController::AAuroraPlayerController()
{
	bReplicates = true;
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
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
	
	AutoRun();
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
	if (InputTag.MatchesTagExact(FAuroraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;	
	}
}

void AAuroraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	// Case #1: Holding not LMB input over target object. Try to use ability bind to this input
	if (!InputTag.MatchesTagExact(FAuroraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
		return;
	}

	// Case #2: Holding LMB over target object (enemy, barrel, etc.). Try to use ability bind to LMB
	if (bTargeting)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
	}

	// Case #3: Holding LMB over the non-target plane. In this case we should move instead using ability until we holding LMB
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		
		if (CursorHit.bBlockingHit)
		{
			CachedDestination = CursorHit.ImpactPoint;
		}

		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

void AAuroraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	// Case #1:
	if (!InputTag.MatchesTagExact(FAuroraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagReleased(InputTag);
		}
		return;
	}

	// Case #2: 
	if (bTargeting)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagReleased(InputTag);
		}
	}

	else
	{
		APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			const FVector PathStartPoint = ControlledPawn->GetActorLocation();
			const FVector PathEndPoint = CachedDestination;

			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, PathStartPoint, PathEndPoint))
			{
				Spline->ClearSplinePoints();
				for (const FVector& PointLoc : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
					DEBUG_DRAW_SPHERE(PointLoc);
				}

				CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
				
				bTargeting = false;
				bAutoRunning = true;
			}
		}

		FollowTime = 0.f;
	}
}

void AAuroraPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	UE_LOG(LogTemp, Warning, TEXT("AAuroraPlayerController::AutoRun()"))
	
	if (APawn* ControllerPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControllerPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControllerPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

#pragma endregion

void AAuroraPlayerController::CursorTrace()
{
	
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
