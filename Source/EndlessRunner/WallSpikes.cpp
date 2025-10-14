// Fill out your copyright notice in the Description page of Project Settings.


#include "WallSpikes.h"

AWallSpikes::AWallSpikes() 
{
	PrimaryActorTick.bCanEverTick = true;

}

void AWallSpikes::BeginPlay()
{
	Super::BeginPlay();

	// Set its speed
	this->GetRootComponent()->ComponentVelocity = FVector(0, 25, 0);
}

void AWallSpikes::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Moving every tick
	SetActorLocation(GetActorLocation() + FVector(0, 350 * DeltaTime, 0), true);
}