#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicCircle.generated.h"

class USphereComponent;

UCLASS()
class AURORA_API AMagicCircle : public AActor
{
	GENERATED_BODY()

public:
	
	AMagicCircle();
	
	
	virtual void Tick(float DeltaTime) override;

protected:
	
	virtual void BeginPlay() override;


	void SetTargetingRadius(float InRadius);

	
	
public:

	UFUNCTION()
	void OnMagicCircleBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult & SweepResult);

	UFUNCTION()
	void OnMagicCircleEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDecalComponent> MagicCircleDecal;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> MagicCircleSphere;

	UFUNCTION(BlueprintCallable)
	void SetMagicCircleDecalSize(float InSize);
	
};
