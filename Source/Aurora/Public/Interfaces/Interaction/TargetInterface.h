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

#pragma region HIGHLIGHT TARGET
	
	virtual void HighLightActor() = 0;
	virtual void UnHighLightActor() = 0;

#pragma endregion

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCombatTarget(AActor* InCombatTarget);

	/** GetCombatTarget
	 *
	 *	Returns target actor of a source actor
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetCombatTarget() const;
};
