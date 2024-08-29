#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TargetInterface.generated.h"

UINTERFACE()
class UTargetInterface : public UInterface
{
	GENERATED_BODY()
};

class AURORA_API ITargetInterface
{
	GENERATED_BODY()

public:

#pragma region HIGHLIGHT INTERFACE
	
	virtual void HighLightActor() = 0;
	virtual void UnHighLightActor() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCombatTarget(AActor* InCombatTarget);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetCombatTarget() const;

#pragma endregion
};
