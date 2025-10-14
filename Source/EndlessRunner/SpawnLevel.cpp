// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnLevel.h"
#include "BaseLevel.h"
#include "Engine.h"
#include "Components/BoxComponent.h"

// Sets default values
ASpawnLevel::ASpawnLevel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawnLevel::BeginPlay()
{
	Super::BeginPlay();
	SpawnLevel(true);
	SpawnLevel(false);
	SpawnLevel(false);
	SpawnLevel(false);
}

// Called every frame
void ASpawnLevel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnLevel::SpawnLevel(bool IsFirst)
{
	// 1000 units apart
	SpawnLocation = FVector(0.0f, 1000.0f, 0.0f);
	SpawnRotation = FRotator(0, 90, 0);

	if (!IsFirst) 
	{
		ABaseLevel* LastLevel = LevelList.Last();
		SpawnLocation = LastLevel->GetSpawnLocation()->GetComponentTransform().GetTranslation();
	}

	RandomLevel = FMath::RandRange(0, Levels.Num());
	// Have a 75% chance of choosing another level if it is the same as the previous level (Avoids repetition)
	if (RandomLevel == PreviousRandomLevel)
	{
		int rand = FMath::RandRange(0, 100);
		if (rand < 75)
		{
			RandomLevel = FMath::RandRange(0, Levels.Num());
		}
	}
	PreviousRandomLevel = RandomLevel;

	ABaseLevel* NewLevel = nullptr;

	// Randomly choose next level
	for (int i = 0; i < Levels.Num(); i++)
	{
		if (RandomLevel == i) {
			NewLevel = GetWorld()->SpawnActor<ABaseLevel>(Levels[i], SpawnLocation, SpawnRotation, SpawnInfo);
		}
	}

	if (NewLevel == nullptr) 
	{
		UE_LOG(LogTemp, Warning, TEXT("New Level is null"));
		return;
	}

	if (NewLevel->GetTrigger()) 
	{
		NewLevel->GetTrigger()->OnComponentBeginOverlap.AddDynamic(this, &ASpawnLevel::OnOverlapBegin);
	}

	LevelList.Add(NewLevel);

	if (LevelList.Num() > 5) 
	{
		LevelList.RemoveAt(0);
	}
}

void ASpawnLevel::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SpawnLevel(false);
}

