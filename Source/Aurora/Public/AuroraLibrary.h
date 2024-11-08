#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuroraLibrary.generated.h"

class UAuroraColors;

UCLASS()
class AURORA_API UAuroraLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	/**
	 * Gets the list of all colors used in this project 
	 *
	 * @return Color data asset with all colors
	 */
	UFUNCTION(BlueprintCallable, Category = "Aurora Library | Data", meta = (DefaultToSelf = "WorldContextObject"))
	static UAuroraColors* GetAuroraColors(const UObject* WorldContextObject);
};
