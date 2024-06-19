#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/Interaction/TargetInterface.h"
#include "AuroraPlayerController.generated.h"


// Input
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

// Interfaces


UCLASS()
class AURORA_API AAuroraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuroraPlayerController();
	
	virtual void PlayerTick(float DeltaTime) override;


protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

#pragma region INPUT
	
private:	

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuroraMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	void Move(const FInputActionValue& InputActionValue);

#pragma endregion

	void CursorTrace();
	
	TScriptInterface<ITargetInterface> LastActor;
	TScriptInterface<ITargetInterface> ThisActor;

	
};
