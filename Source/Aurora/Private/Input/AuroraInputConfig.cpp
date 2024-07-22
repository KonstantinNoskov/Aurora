#include "Input/AuroraInputConfig.h"

const UInputAction* UAuroraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag,
	bool bLogNotFound) const
{
	for (const FAuroraInputAction Action : AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}

		if (bLogNotFound)
		{
			UE_LOG(LogTemp, Error,TEXT("Can't find ability input action for input tag [%s], on InputConfig [%s]"),
				*InputTag.ToString(), *GetNameSafe(this))
		}
	}

	return nullptr;
}
