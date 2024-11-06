#include "Actor/PointCollection.h"

#include "AbilitySystem/AuroraAbilitySystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

APointCollection::APointCollection()
{
	PrimaryActorTick.bCanEverTick = false;

#pragma region Points Setup
	
	Point_0 = CreateDefaultSubobject<USceneComponent>("Point_0");
	ImmutablePoints.Add(Point_0);
	SetRootComponent(Point_0);

	Point_1 = CreateDefaultSubobject<USceneComponent>("Point_1");
	ImmutablePoints.Add(Point_1);
	Point_1->SetupAttachment(GetRootComponent());
	
	Point_2 = CreateDefaultSubobject<USceneComponent>("Point_2");
	ImmutablePoints.Add(Point_2);
	Point_2->SetupAttachment(GetRootComponent());
		
	Point_3 = CreateDefaultSubobject<USceneComponent>("Point_3");
	ImmutablePoints.Add(Point_3);
	Point_3->SetupAttachment(GetRootComponent());
		
	Point_4 = CreateDefaultSubobject<USceneComponent>("Point_4");
	ImmutablePoints.Add(Point_4);
	Point_4->SetupAttachment(GetRootComponent());
		
	Point_5 = CreateDefaultSubobject<USceneComponent>("Point_5");
	ImmutablePoints.Add(Point_5);
	Point_5->SetupAttachment(GetRootComponent());
		
	Point_6 = CreateDefaultSubobject<USceneComponent>("Point_6");
	ImmutablePoints.Add(Point_6);
	Point_6->SetupAttachment(GetRootComponent());
		
	Point_7 = CreateDefaultSubobject<USceneComponent>("Point_7");
	ImmutablePoints.Add(Point_7);
	Point_7->SetupAttachment(GetRootComponent());
		
	Point_8 = CreateDefaultSubobject<USceneComponent>("Point_8");
	ImmutablePoints.Add(Point_8);
	Point_8->SetupAttachment(GetRootComponent());
		
	Point_9 = CreateDefaultSubobject<USceneComponent>("Point_9");
	ImmutablePoints.Add(Point_9);
	Point_9->SetupAttachment(GetRootComponent());
		
	Point_10 = CreateDefaultSubobject<USceneComponent>("Point_10");
	ImmutablePoints.Add(Point_10);
	Point_10->SetupAttachment(GetRootComponent());


#pragma endregion
	
}

void APointCollection::BeginPlay()
{
	Super::BeginPlay();
}


TArray<USceneComponent*> APointCollection::GetGroundPoints(const FVector& GroundLocation, int32 NumPoints, float YawOverride)
{
	checkf(ImmutablePoints.Num() >= NumPoints, TEXT("[%hs]: Attempted to access ImmutablePoints out of bounds."), __FUNCTION__);

	TArray<USceneComponent*> ArrayCopy;

	for (USceneComponent* Point : ImmutablePoints)
	{
		if (ArrayCopy.Num() >= NumPoints) return ArrayCopy;

		if (Point != Point_0)
		{
			FVector ToPoint = Point->GetComponentLocation() - Point_0->GetComponentLocation();
			ToPoint = ToPoint.RotateAngleAxis(YawOverride, FVector::UpVector);
			Point->SetWorldLocation(Point_0->GetComponentLocation() + ToPoint);
		}

		// Line Trace Range Locations. From 500.f above origin point to 500.f below it.
		const FVector RaisedLocation = FVector(Point->GetComponentLocation().X, Point->GetComponentLocation().Y, Point->GetComponentLocation().Z + 500.f);
		const FVector LoweredLocation = FVector(Point->GetComponentLocation().X, Point->GetComponentLocation().Y, Point->GetComponentLocation().Z - 500.f);
		
		FHitResult HitResult;

		// Adding all characters to ignore list, so we can use only surface to adjust our points
		TArray<AActor*> IgnoreActors;
		UAuroraAbilitySystemLibrary::GetAlivePlayersWithinRadius(this, IgnoreActors, TArray<AActor*>(), 1500.f, GetActorLocation());
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActors(IgnoreActors);

		// Fire line trace from the RaisedLocation to the LoweredLocation 
		GetWorld()->LineTraceSingleByProfile(HitResult, RaisedLocation, LoweredLocation, FName("BlockAll"), QueryParams);

		// If LineTrace hit some surface (not character), adjusting Z-location and Rotation of the Point
		const FVector AdjustedLocation = FVector(Point->GetComponentLocation().X, Point->GetComponentLocation().Y, HitResult.ImpactPoint.Z);
		Point->SetWorldLocation(AdjustedLocation);
		Point->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal));

		ArrayCopy.Add(Point);
	}

	return ArrayCopy;
}


