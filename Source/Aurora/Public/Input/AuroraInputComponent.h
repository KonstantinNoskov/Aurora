#pragma once

#include "CoreMinimal.h"
#include "AuroraInputConfig.h"
#include "EnhancedInputComponent.h"
#include "AuroraInputComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURORA_API UAuroraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	UAuroraInputComponent();

protected:
	
	virtual void BeginPlay() override;

public:
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(
		const UAuroraInputConfig* InputConfig,
		UserClass* Object,
		PressedFuncType PressedFunc,	// action pressed
		ReleasedFuncType ReleasedFunc,	// action released
		HeldFuncType HeldFunc			// action held
		)
	{
		check(InputConfig)

		for (const FAuroraInputAction& Action : InputConfig->AbilityInputActions)
		{
			if (Action.InputAction && Action.InputTag.IsValid())
			{
				if (PressedFunc)	BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
				if (HeldFunc)		BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
				if (ReleasedFunc)	BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}
		}
	}
};
