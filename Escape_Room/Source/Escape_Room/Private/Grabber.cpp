// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include <GameFramework/Actor.h>
#include <Components/PrimitiveComponent.h>
#include <GameFramework/PlayerController.h>
#include <Engine/World.h>
#include <DrawDebugHelpers.h>
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();
	BindInputComponentBehaviours();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if the physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{

		FVector PlayerViewPointLocation;
		FRotator PlayerViewPointRotation;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
			OUT PlayerViewPointLocation,
			OUT PlayerViewPointRotation
		);

		FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector()*Reach);

		// move the object the physics handle is holding
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}

}

void UGrabber::Grab()
{
	if (!Grabbing)
	{
		auto Hit = GetFirstPhysicsBodyInReach();
		auto ActorHit = Hit.GetActor();
		auto ComponentToGrab = Hit.GetComponent();

		if (ActorHit)
		{
			PhysicsHandle->GrabComponent(
				ComponentToGrab,
				NAME_None,
				ComponentToGrab->GetOwner()->GetActorLocation(),
				true
			);
			Grabbing = true;
		}
	}
	else {
		Grabbing = false;
	}
}

void UGrabber::Release()
{
	if (!Grabbing) 
	{
		PhysicsHandle->ReleaseComponent();
	}
}

void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s must have a PhysicsHandle component"), *GetOwner()->GetName());
	}

}

void UGrabber::BindInputComponentBehaviours()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s must have a InputComponent component"), *GetOwner()->GetName());
	}
}



FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector()*Reach);

	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		FCollisionQueryParams(FName(TEXT("")), false, GetOwner())
	))
	{
		UE_LOG(LogTemp, Warning, TEXT("Object Hit %s"), *Hit.Actor->GetName());

		return Hit;
	}
	return Hit;
}

