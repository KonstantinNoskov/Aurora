
#include "Actor/AuroraEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuroraAttributeSet.h"
#include "Components/SphereComponent.h"

AAuroraEffectActor::AAuroraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
	
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
}

void AAuroraEffectActor::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuroraEffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AAuroraEffectActor::OnEndOverlap);
}

void AAuroraEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	// TODO: Change this to apply a Gameplay Effect. For now, using const_cast as a hack
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UAuroraAttributeSet* AuroraAttributeSet = Cast<UAuroraAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UAuroraAttributeSet::StaticClass()));
		
		UAuroraAttributeSet* MutableAuroraAttributeSet = const_cast<UAuroraAttributeSet*>(AuroraAttributeSet);
		MutableAuroraAttributeSet->SetHealth(AuroraAttributeSet->GetHealth() + 25.f);
		Destroy();
	}
}

void AAuroraEffectActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}


