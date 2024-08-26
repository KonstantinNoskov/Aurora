#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "AuroraAIController.generated.h"

class UBehaviorTreeComponent;

UCLASS()
class AURORA_API AAuroraAIController : public AAIController
{
	GENERATED_BODY()

public:
	
	AAuroraAIController();

protected:
	
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
	
};
