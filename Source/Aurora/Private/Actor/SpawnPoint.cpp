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

#pragma region PostEdit Properies Changes

#if WITH_EDITOR

void ASpawnPoint::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	
	UpdatePreviewMesh(ChangedPropertyName);
}
void ASpawnPoint::UpdatePreviewMesh(const FName ChangedPropertyName)
{
	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(ASpawnPoint, EnemySpawnClass))
	{
		// If enemy class set to "None" 
		if (!EnemySpawnClass)
		{	
			PreviewMesh->SetSkeletalMeshAsset(nullptr); // disable preview mesh
			BillBoard->SetSprite(InitialSprite); // enable billboard sprite
		}

		// If set existing enemy class
		if (EnemySpawnClass)
		{
			// Adjust Capsule size corresponding to an enemy size
			AAuroraEnemy* Enemy = EnemySpawnClass->GetDefaultObject<AAuroraEnemy>();
			Root->SetCapsuleHalfHeight(Enemy->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
			Root->SetCapsuleRadius(Enemy->GetCapsuleComponent()->GetScaledCapsuleRadius());

			// Set Enemy preview mesh, locate and rotate it accordingly to a spawn point 
			const FVector PreviewMeshLocation = Root->GetComponentLocation() - FVector(0.f,0.f, Root->GetScaledCapsuleHalfHeight());
			PreviewMesh->SetSkeletalMeshAsset(Enemy->GetMesh()->GetSkeletalMeshAsset());
			PreviewMesh->SetWorldLocation(GetActorLocation());
			PreviewMesh->SetWorldLocation(PreviewMeshLocation);

			// Disable Billboard Sprite. You don't see the sprite behind the enemy preview mesh anyway. 
			BillBoard->SetSprite(nullptr);
		}
	}
}

#endif

#pragma endregion

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
