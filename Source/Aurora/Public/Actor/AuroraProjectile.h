#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuroraProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class AURORA_API AAuroraProjectile : public AActor
{
	GENERATED_BODY()

public:
	AAuroraProjectile();

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
