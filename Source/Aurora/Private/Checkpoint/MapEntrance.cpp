#include "Checkpoint/MapEntrance.h"

#include "Components/SphereComponent.h"
#include "GameModes/AuroraGameModeBase.h"
#include "Interfaces/Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"


AMapEntrance::AMapEntrance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

	PrimaryActorTick.bCanEverTick = false;
	Sphere->SetupAttachment(MoveToComponent);
}


void AMapEntrance::BeginPlay()
{
	Super::BeginPlay();
}

void AMapEntrance::HighLightActor_Implementation()
{
	CheckPointMesh->SetRenderCustomDepth(true);
}

void AMapEntrance::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		bReached = true;

		if (AAuroraGameModeBase* AuroraGM = Cast<AAuroraGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			// TODO: Save the World we are traveling to.
			AuroraGM->SaveWorldState(GetWorld(), DestinationMap.ToSoftObjectPath().GetAssetName());
		}
		
		IPlayerInterface::Execute_SaveProgress(OtherActor, DestinationPlayerStartTag);

		UGameplayStatics::OpenLevelBySoftObjectPtr(this, DestinationMap);
	}
}

void AMapEntrance::LoadActor_Implementation()
{
	// Do nothing when loading a Map Entrance
}



