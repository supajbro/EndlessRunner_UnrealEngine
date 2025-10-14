// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ARunnerCharacter::ARunnerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	GetCapsuleComponent()->SetCollisionResponseToChannel
	(ECC_GameTraceChannel1, ECR_Overlap);

	// Controller cannot rotate
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	SideViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Side View Camera"));
	SideViewCamera->bUsePawnControlRotation = false;

	// Set characters movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.f, 0.0f);
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.8f;
	GetCharacterMovement()->JumpZVelocity = 1000.0f;
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;

	tempPos = GetActorLocation();
	zPos = tempPos.Z + 300.0f;
}

// Called when the game starts or when spawned
void ARunnerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	CanMove = true;
}

// Called every frame
void ARunnerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bool bIsGrounded = GetCharacterMovement()->IsMovingOnGround();

	if (bIsGrounded) {
		TimeSinceLeftGround = 0.f;
	}
	else {
		TimeSinceLeftGround += DeltaTime;
	}

	tempPos = GetActorLocation();
	tempPos.X -= 850.f;
	tempPos.Z = zPos;
	SideViewCamera->SetWorldLocation(tempPos);

	FallingGravity();
}

// Called to bind functionality to input
void ARunnerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveRight", this, &ARunnerCharacter::MoveRight);
}

void ARunnerCharacter::Jump()
{
	if (GetCharacterMovement()->IsMovingOnGround() || TimeSinceLeftGround <= KoyoteTime) {
		Super::Jump();
	}
}

void ARunnerCharacter::MoveRight(float value)
{
	if (!CanMove) {
		return;
	}

	AddMovementInput(FVector(0.0f, 1.0f, 0.0f), value);
}

void ARunnerCharacter::FallingGravity()
{
	if (!GetCharacterMovement()->IsFalling()) 
	{
		return;
	}

	FVector tempVel = GetCharacterMovement()->Velocity;
	tempVel.Z -= ExtraFallingVel;
	GetCharacterMovement()->Velocity = tempVel;
}

void ARunnerCharacter::RestartLevel()
{

}

void ARunnerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

