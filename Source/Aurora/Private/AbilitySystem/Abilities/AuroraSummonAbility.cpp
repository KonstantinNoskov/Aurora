#include "AbilitySystem/Abilities/AuroraSummonAbility.h"

#include "Characters/AuroraEnemy.h"
#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UAuroraSummonAbility::GetSpawnLocations()
{
	
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread = SpawnSpread / NumMinions;
	
	// Right of spread arrow
	const FVector RightOfSpread = Forward.RotateAngleAxis(SpawnSpread / 2.f, FVector::UpVector);

	TArray<FVector> SpawnLocations;
	
	for (int32 i = 0; i < NumMinions; i++)
	{
		
		const FVector Direction = RightOfSpread.RotateAngleAxis(-DeltaSpread * i, FVector::UpVector);
		FVector ChosenSpawnLocation = Location + Direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);

		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(
			Hit,
			ChosenSpawnLocation + FVector(0.f, 0.f, 400.f),
			ChosenSpawnLocation - FVector(0.f, 0.f, 400.f),
			ECC_Visibility);
		
		if (Hit.bBlockingHit)
		{
			ChosenSpawnLocation = Hit.ImpactPoint;
		}
		
		SpawnLocations.Add(ChosenSpawnLocation);
		
		if (bDebug)
		{
			
			DrawDebugSphere(GetWorld(), ChosenSpawnLocation, 10.f , 12, FColor::Red, false, 3.f);
			UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(),Location,Location + Direction * MaxSpawnDistance,4.f,FColor::Purple,3.f,2.f);
			DrawDebugSphere(GetWorld(), Location + Direction * MinSpawnDistance, 5.f , 12, FColor::Cyan, false, 3.f);
			DrawDebugSphere(GetWorld(), Location + Direction * MaxSpawnDistance, 5.f , 12, FColor::Cyan, false, 3.f);
		}
	}
	
	return SpawnLocations;
}

const AAuroraEnemy* UAuroraSummonAbility::GetRandomMinionClass()
{
	const int32 Selection = FMath::RandRange(0, MinionClasses.Num() - 1);
	
	const AAuroraEnemy* MinionDefaultObject = MinionClasses[Selection]->GetDefaultObject<AAuroraEnemy>();
	
	return MinionDefaultObject;
}
