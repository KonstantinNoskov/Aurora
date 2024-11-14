#pragma once

#include "CoreMinimal.h"
#include "AuroraAbilityTypes.h"
#include "GameFramework/Actor.h"
#include "AuroraProjectile.generated.h"

class UNiagaraSystem;
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

	UFUNCTION(BlueprintCallable)
	virtual void OnHit();

public:
	virtual void Destroyed() override;

	/*UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;*/

private:
	
	

protected:

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	/***
	 *  Check if all condition for triggering overlap logic  is valid.
	 *  It contains:
	 *		- Check for valid source actor ability system
	 *		- Compare source avatar actor with a overlap actor to avoid the projectile to overlap the owner
	 *		- Check to avoid the projectile to overlap friendly actors
	 *
	 *  @param OtherActor - The actor that was overlap.
	 *
	 *  @return Whether projectile overlap logic should be triggered  
	 */
	bool IsValidOverlap(const AActor* OtherActor) const;
	
public:

	/*
	 * 
	 */
	FORCEINLINE USphereComponent* GetSphereCollision() { return Sphere; }
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() { return ProjectileMovement; }

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FDamageEffectParams DamageEffectParams;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FProjectileBehaviorParams ProjectileBehaviorParams;
	
private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 15.f;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopingSound;

protected:

	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;
	
	bool bHit = false;

public:

	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetSceneComponent;
};
