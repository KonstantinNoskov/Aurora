﻿#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalc_Damage.generated.h"

UCLASS()
class AURORA_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:

	UExecCalc_Damage();
	
	void DetermineDebuff(
		const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		const FGameplayEffectSpec& Spec,
		FAggregatorEvaluateParameters EvaluationParams,
		const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>&InTagsToCaptureDefs) const;

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	                                    FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
	float  CalculateRadialDamage(FGameplayEffectContextHandle& InEffectContextHandle, const float InDamage, const AActor* InTargetAvatar) const;
};
