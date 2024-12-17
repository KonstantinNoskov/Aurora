#pragma once

#include "CoreMinimal.h"
#include "CheckPoint.h"
#include "MapEntrance.generated.h"

UCLASS()
class AURORA_API AMapEntrance : public ACheckPoint
{
	GENERATED_BODY()

public:
	
	AMapEntrance(const FObjectInitializer& ObjectInitializer);

protected:

	virtual void BeginPlay() override;

	virtual void HighLightActor_Implementation() override;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> DestinationMap;

	UPROPERTY(EditAnywhere)
	FName DestinationPlayerStartTag;

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

public:
	
	virtual void LoadActor_Implementation() override;
};
