// Fill out your copyright notice in the Description page of Project Settings.

#include "PositionReport.h"
#include <GameFramework/Actor.h>

// Sets default values for this component's properties
UPositionReport::UPositionReport()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPositionReport::BeginPlay()
{
	Super::BeginPlay();


	UE_LOG(LogTemp, Warning, TEXT("Position Reporter"));

	AActor* act = GetOwner();
	FString name = act->GetName();
	FVector pos = act->GetActorLocation();
	FString posStr = pos.ToCompactString();
	UE_LOG(LogTemp, Warning, TEXT("Position Reporter: Actor name is %s & position is %s"), *name, *posStr);

	FRotator rotation = act->GetActorRotation();
	FRotator rot = FRotator(0.0f, 90.0f, 0.0f);
	FRotator both = rotation + rot;
	act->SetActorRotation(both);
}


// Called every frame
void UPositionReport::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

