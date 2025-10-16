// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"
#include "RunnerCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// The moving platform
	MovingCube = CreateDefaultSubobject<UBoxComponent>(TEXT("MovingCube"));
	MovingCube->SetupAttachment(RootComponent);

	// Enable collision for overlap detection
	MovingCube->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MovingCube->SetCollisionResponseToAllChannels(ECR_Overlap);

	// Start position marker
	StartPosition = CreateDefaultSubobject<UBoxComponent>(TEXT("StartPosition"));
	StartPosition->SetupAttachment(RootComponent);
	StartPosition->SetBoxExtent(FVector(20.f)); // Small visual marker

	// End position marker
	EndPosition = CreateDefaultSubobject<UBoxComponent>(TEXT("EndPosition"));
	EndPosition->SetupAttachment(RootComponent);
	EndPosition->SetBoxExtent(FVector(20.f)); // Small visual marker
}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	SetActorLocation(GetStartPosition()->GetComponentTransform().GetTranslation());
	MovingCube->OnComponentBeginOverlap.AddDynamic(this, &AMovingPlatform::OnOverlapBegin);
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveToStartPosition(DeltaTime);
	MoveToEndPosition(DeltaTime);
}

void AMovingPlatform::MoveToStartPosition(float DeltaTime)
{
	if (bMoveToEndPosition)
	{
		return;
	}
}

void AMovingPlatform::MoveToEndPosition(float DeltaTime)
{
	if (!bMoveToEndPosition)
	{
		return;
	}

	// Move to end position
	FVector CurrentLocation = MovingCube->GetComponentLocation();
	FVector EndPos = GetEndPosition()->GetComponentTransform().GetTranslation();
	FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, EndPos, DeltaTime, MoveSpeed);
	MovingCube->SetWorldLocation(NewLocation);
}

void AMovingPlatform::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!OtherActor)
	{
		return;
	}

	ARunnerCharacter* Player = Cast<ARunnerCharacter>(OtherActor);
	if (Player)
	{
		bMoveToEndPosition = true;
	}
}

UBoxComponent* AMovingPlatform::GetStartPosition()
{
	return StartPosition;
}

UBoxComponent* AMovingPlatform::GetEndPosition()
{
	return EndPosition;
}

