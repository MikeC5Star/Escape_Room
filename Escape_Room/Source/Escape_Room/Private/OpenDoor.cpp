// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include <Components/PrimitiveComponent.h>
#include <GameFramework/Actor.h>
#include <Engine/World.h>

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetTotalMassOnTriggerPlate() > TriggerMassNeeded)
	{
		OpenDoor();
		LastOpenTime = GetWorld()->GetTimeSeconds();
	}

	if (GetWorld()->GetTimeSeconds() > LastOpenTime + DoorCloseDelay) {
		CloseDoor();
	}

}

void UOpenDoor::OpenDoor()
{
	Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}

float UOpenDoor::GetTotalMassOnTriggerPlate() const
{
	TArray<AActor*> Actors;
	PressurePlate->GetOverlappingActors(Actors);
	float TotalMass = 0.0f;
	for (AActor* act : Actors)
	{
		float mass = act->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		TotalMass += mass;
		UE_LOG(LogTemp, Warning, TEXT("Actor: %s  has Mass: %f Total Mass is now %f kg"), *act->GetName(), mass, TotalMass);

	
	}
	return TotalMass;
}

