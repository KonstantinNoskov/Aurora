#pragma once

#include "CoreMinimal.h"
#include "AuroraProjectile.h"
#include "AuroraFireBall.generated.h"

UCLASS()
class AURORA_API AAuroraFireBall : public AAuroraProjectile
{
	GENERATED_BODY()

public:
	
	AAuroraFireBall();

protected:
	
	virtual void BeginPlay() override;

private:
	
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

protected:
	
	virtual void OnHit() override;

public:

	UFUNCTION(BlueprintImplementableEvent)
	void StartOutgoingTimeLline();

	

public:

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> ReturnToActor;
	
	UPROPERTY(BlueprintReadWrite)
	FDamageEffectParams ExplosionDamageEffectParams;
};
