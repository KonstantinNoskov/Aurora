#include "Actor/SpawnPoint.h"

#include "Characters/AuroraEnemy.h"
#include "Components/BillboardComponent.h"


ASpawnPoint::ASpawnPoint(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<UCapsuleComponent>("Root");
	SetRootComponent(Root);

	// Billboard
	BillBoard = CreateDefaultSubobject<UBillboardComponent>("Billboard");
	BillBoard->SetupAttachment(GetRootComponent());
	InitialSprite = BillBoard->Sprite;
	
	// Preview Mesh
	PreviewMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Preview Mesh");
	PreviewMesh->SetupAttachment(GetRootComponent());
}

void ASpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	PreviewMesh->SetSkeletalMeshAsset(nullptr);
	
	HandleSpawnEnemy();
}

#if WITH_EDITOR
void ASpawnPoint::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(ASpawnPoint, EnemySpawnClass))
	{
		if (!EnemySpawnClass)
		{
			PreviewMesh->SetSkeletalMeshAsset(nullptr);
			BillBoard->SetSprite(InitialSprite);
		}
		
		if (EnemySpawnClass)
		{
			AAuroraEnemy* Enemy = EnemySpawnClass->GetDefaultObject<AAuroraEnemy>();
			Root->SetCapsuleHalfHeight(Enemy->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
			Root->SetCapsuleRadius(Enemy->GetCapsuleComponent()->GetScaledCapsuleRadius());

			// Preview Mesh Location
			const FVector PreviewMeshLocation = Root->GetComponentLocation() - FVector(0.f,0.f, Root->GetScaledCapsuleHalfHeight());
			PreviewMesh->SetSkeletalMeshAsset(Enemy->GetMesh()->GetSkeletalMeshAsset());
			PreviewMesh->SetWorldLocation(GetActorLocation());
			PreviewMesh->SetWorldLocation(PreviewMeshLocation);

			// Disable Billboard Sprite
			BillBoard->SetSprite(nullptr);
		}
	}
}
#endif

void ASpawnPoint::HandleSpawnEnemy()	
{	
	// Valid Check
	if (!bShouldSpawn) return;
	
	// Spawn Params
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	// Spawn Enemy
	SpawnedEnemy = GetWorld()->SpawnActor<AAuroraEnemy>(EnemySpawnClass, GetActorLocation(), GetActorRotation(), SpawnParams);
	
	if (IsValid(SpawnedEnemy))
	{
		SpawnedEnemy->GetOnDeathDelegate().AddDynamic(this, &ASpawnPoint::OnSpawnedEnemyDied);
	}
}

void ASpawnPoint::OnSpawnedEnemyDied(AActor* DeadActor)
{	
	bShouldSpawn = false;
	SpawnedEnemy = nullptr;
}
