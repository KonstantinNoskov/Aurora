#include "Characters/AuroraCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

AAuroraCharacter::AAuroraCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Camera
	AuroraSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("AuroraSpringArm");
	AuroraSpringArmComponent->SetupAttachment(GetRootComponent());
	AuroraSpringArmComponent->TargetArmLength = 400.f;
	AuroraSpringArmComponent->bUsePawnControlRotation = true;
	
	AuroraCameraComponent = CreateDefaultSubobject<UCameraComponent>("AuroraCamera");
	AuroraCameraComponent->SetupAttachment(AuroraSpringArmComponent, USpringArmComponent::SocketName);
	AuroraCameraComponent->bUsePawnControlRotation = false;

	// Orientation 
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void AAuroraCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuroraCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAuroraCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

		
	
}


