#include "AbilitySystem/Data/AbilityInfo.h"

#include "Aurora/AuroraLogChannels.h"

FAuroraAbilityInfo UAbilityInfo::FindAbilityInfoByTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for (const FAuroraAbilityInfo& Info : AbilityInformation)
	{
		if (Info.AbilityTag.MatchesTagExact(AbilityTag))
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogAurora, Error, TEXT("Can't find info for AbilityTag [%s] on AbilityInfo [%s]"), *AbilityTag.ToString(), *GetNameSafe(this));
	}

	return FAuroraAbilityInfo();
}
