#include "Actor/AuroraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Kismet/KismetMathLibrary.h"

AAuroraEffectActor::AAuroraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuroraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
	SetInitialTransform();
}

void AAuroraEffectActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	ItemMovement(DeltaSeconds);
}

void AAuroraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
 {
	
	// If we dont want to apply this effect to the enemy - return
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectToEnemies) return;
	
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	
	if (!TargetASC) return;

	// Valid gameplay effect class should be set. 
	//checkf (GameplayEffectClass, TEXT("GameplayEffectClass not set in. Class: AuroraEffectActor"))
	if (!GameplayEffectClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayEffectClass not set."))
		return;
	}
	
	// Effect context init. 
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);

	// Apply effect to target actor and store it as an active effect.   
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	
	// If effect has an infinite duration, we should store the references to actor's ability systems affected by active effect 
	// in case we want to remove the effects later. 
	const bool bIsInfiniteDuration = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfiniteDuration && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandlesMap.Add(ActiveEffectHandle, TargetASC);
	}

	if (!bIsInfiniteDuration)
	{
		Destroy();
	}
}
void AAuroraEffectActor::OnOverlap(AActor* TargetActor)
{

	// If we dont want enemy to overlap this effect (fire area, etc.) - return
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectToEnemies) return;
	
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}
void AAuroraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	// If we dont want enemy to overlap this effect, we dont want to trigger EndOverlap event neither, so - return
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectToEnemies) return;
	
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
	
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap )
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC)) return;

		// Stores an EffectHandles we're about to remove
		TArray<FActiveGameplayEffectHandle> HandlesToRemove;

		// We can't remove an objects we're looping through safely so we add the active effects to the array to remove them after.
		for	(TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePair : ActiveEffectHandlesMap)
		{
			if (TargetASC == HandlePair.Value)
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}

		// Loop through active effect removal list. This is where we actually removing effects. 
		for (FActiveGameplayEffectHandle& Handle : HandlesToRemove)
		{
			ActiveEffectHandlesMap.FindAndRemoveChecked(Handle);
		}
	}
}
void AAuroraEffectActor::StartSinusoidalMovement()
{
	bSinusoidalMovement = true;
	InitialLocation = GetActorLocation();
	CalculatedLocation = InitialLocation;
}
void AAuroraEffectActor::StartRotation()
{
	bRotates = true;
	CalculatedRotation = GetActorRotation();
}

void AAuroraEffectActor::SetInitialTransform()
{
	CalculatedRotation = GetActorRotation();
	InitialLocation = GetActorLocation();
	CalculatedLocation = InitialLocation;
	
}

void AAuroraEffectActor::ItemMovement(float DeltaTime)
{
	const float SinePeriod = PI * 2 / SinePeriodConstant;
	RunningTime = RunningTime > SinePeriod ? 0 : RunningTime + DeltaTime;

	if (bRotates)
	{
		const FRotator DeltaRotation(0.f, DeltaTime * RotationRate, 0.f);
		CalculatedRotation = UKismetMathLibrary::ComposeRotators(CalculatedRotation, DeltaRotation);
	}

	if (bSinusoidalMovement)
	{
		const float Sine = SineAmplitude * FMath::Sin(RunningTime * SinePeriodConstant);
		CalculatedLocation = InitialLocation + FVector(0.f, 0.f, Sine);
	}
}



