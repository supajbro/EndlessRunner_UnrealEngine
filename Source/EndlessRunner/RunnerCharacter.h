// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "RunnerCharacter.generated.h"

UCLASS()
class ENDLESSRUNNER_API ARunnerCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* SideViewCamera;

	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* CameraBoom;

public:
	// Sets default values for this character's properties
	ARunnerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Jump() override;

protected:
	void MoveRight(float value);
	void FallingGravity(float DeltaTime);

public:
	class UCameraComponent* GetSideViewCameraComponent() const 
	{
		return SideViewCamera;
	}

	void RestartLevel();

	void CameraControls(float DeltaTime);

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	float zPos;
	FVector tempPos = FVector();
	bool CanMove;

	float FallGravityMultiplier;
	UPROPERTY(EditAnywhere, Category = "Falling")
	float FallGravityScaler = 5.f;
	UPROPERTY(EditAnywhere, Category = "Falling")
	float FallGravityMultiplierMin = 10.f;
	UPROPERTY(EditAnywhere, Category = "Falling")
	float FallGravityMultiplierMax = 100.f;

	// Koyote time
	float TimeSinceLeftGround = 0.f;
	bool bWasGroundedLastFrame = false;

	UPROPERTY(EditAnywhere, Category = "Jumping")
	float KoyoteTime = .2f;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UDistanceTrackerComponent* DistanceTracker;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UDistanceWidget* DistanceWidget;

	// Widget Blueprint class reference
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UDistanceWidget> DistanceWidgetClass;

public:

	// Powerups
	UPROPERTY(EditAnywhere, Category = "Powerup")
	bool bDoubleJump;
	bool bHasDoubleJumped;
};
