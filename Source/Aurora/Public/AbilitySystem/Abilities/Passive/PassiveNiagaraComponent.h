#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "PassiveNiagaraComponent.generated.h"


class UAuroraAbilitySystemComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURORA_API UPassiveNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:
	
	UPassiveNiagaraComponent();

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PassiveSpellTag;

protected:
	
	virtual void BeginPlay() override;

	void OnPassiveActivate(const FGameplayTag& InAbilityTag, bool bActivate);
	
	void ActivateIfEquipped(UAuroraAbilitySystemComponent* AuroraASC);
};



