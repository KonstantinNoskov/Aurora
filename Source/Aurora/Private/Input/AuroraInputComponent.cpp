#include "Input/AuroraInputComponent.h"

UAuroraInputComponent::UAuroraInputComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

}

void UAuroraInputComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


void UAuroraInputComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

