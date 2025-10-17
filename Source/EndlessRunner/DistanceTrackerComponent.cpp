// Fill out your copyright notice in the Description page of Project Settings.


#include "DistanceTrackerComponent.h"

// Sets default values for this component's properties
UDistanceTrackerComponent::UDistanceTrackerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDistanceTrackerComponent::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = GetOwner()->GetActorLocation();
}


// Called every frame
void UDistanceTrackerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector CurrentLocation = GetOwner()->GetActorLocation();
	float DistanceCm = FVector::Dist(CurrentLocation, StartLocation);
	DistanceMeters = DistanceCm / 100.0f; // convert to meters
}

