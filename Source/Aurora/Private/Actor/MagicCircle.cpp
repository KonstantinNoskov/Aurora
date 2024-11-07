#include "Actor/MagicCircle.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "Interfaces/Interaction/TargetInterface.h"

AMagicCircle::AMagicCircle()
{
	PrimaryActorTick.bCanEverTick = true;

	MagicCircleSphere = CreateDefaultSubobject<USphereComponent>("TargetingSphere");
	SetRootComponent(MagicCircleSphere);
	MagicCircleSphere->SetCollisionResponseToChannels(ECR_Ignore);
	MagicCircleSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	MagicCircleDecal = CreateDefaultSubobject<UDecalComponent>("MagicCircleDecal");
	MagicCircleDecal->SetupAttachment(GetRootComponent());
}

void AMagicCircle::BeginPlay()
{
	Super::BeginPlay();

	MagicCircleSphere->OnComponentBeginOverlap.AddDynamic(this, &AMagicCircle::OnMagicCircleBeginOverlap);
	MagicCircleSphere->OnComponentEndOverlap.AddDynamic(this, &AMagicCircle::OnMagicCircleEndOverlap);
}

void AMagicCircle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMagicCircle::SetTargetingRadius(float InRadius)
{
	MagicCircleSphere->SetSphereRadius(InRadius);
	MagicCircleDecal->DecalSize = FVector(InRadius);
}

void AMagicCircle::OnMagicCircleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                           const FHitResult& SweepResult)
{
	if (ITargetInterface* Target = Cast<ITargetInterface>(OtherActor))
	{
		Target->HighLightActor();
	}
}

void AMagicCircle::OnMagicCircleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ITargetInterface* Target = Cast<ITargetInterface>(OtherActor))
	{
		Target->UnHighLightActor();
	}
}

void AMagicCircle::SetMagicCircleDecalSize(float InSize)
{
	MagicCircleDecal->DecalSize = FVector(InSize);
}


