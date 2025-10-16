// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

// Find furthest left and furthest right position
// Speed of movement
// Object we are moving
// When player is on platform, move it to its end position
// Else be on start position

class UBoxComponent;

UCLASS()
class ENDLESSRUNNER_API AMovingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void MoveToStartPosition(float DeltaTime);
	void MoveToEndPosition(float DeltaTime);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

protected:
	bool bMoveToEndPosition;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Moving Platform")
	float MoveSpeed = 100.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Moving Platform")
	UBoxComponent* MovingCube;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Moving Platform", meta = (MakeEditWidget = "true"))
	UBoxComponent* StartPosition;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Moving Platform", meta = (MakeEditWidget = "true"))
	UBoxComponent* EndPosition;

public:
	UBoxComponent* GetStartPosition();
	UBoxComponent* GetEndPosition();
};
