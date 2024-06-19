#pragma once

#include "CoreMinimal.h"
#include "AuroraCharacterBase.h"
#include "Interfaces/Interaction/TargetInterface.h"
#include "AuroraEnemy.generated.h"

UCLASS()
class AURORA_API AAuroraEnemy : public AAuroraCharacterBase, public ITargetInterface
{
	GENERATED_BODY()

public:
	AAuroraEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

#pragma region TARGET INTERFACE
	
	virtual void HighLightActor() override;
	virtual void UnHighLightActor() override;

	UPROPERTY(BlueprintReadOnly)
	bool bHighlighted = false;

#pragma endregion

};
