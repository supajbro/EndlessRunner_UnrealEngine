// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Spikes.h"
#include "WallSpikes.h"
#include "Engine.h"
#include "DistanceTrackerComponent.h"
#include "DistanceWidget.h"
#include "Blueprint/UserWidget.h"

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

	DistanceTracker = CreateDefaultSubobject<UDistanceTrackerComponent>(TEXT("DistanceTracker"));

	// Create a camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance
	CameraBoom->bUsePawnControlRotation = false; // Disable rotation with the pawn
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bInheritRoll = false;
	CameraBoom->bDoCollisionTest = false; // Disable collision tests to prevent it from shortening
	CameraBoom->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f)); // Set a fixed rotation

	// Init camera
	SideViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Side View Camera"));
	SideViewCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCamera->bUsePawnControlRotation = false;

	if (SideViewCamera)
	{
		SideViewCamera->bUsePawnControlRotation = false;
	}
}

// Called when the game starts or when spawned
void ARunnerCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ARunnerCharacter::OnOverlapBegin);
	
	CanMove = true;

	if (DistanceWidgetClass)
	{
		DistanceWidget = CreateWidget<UDistanceWidget>(GetWorld(), DistanceWidgetClass);
		if (DistanceWidget)
		{
			DistanceWidget->AddToViewport();
		}
	}
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

	FallingGravity(DeltaTime);

	if (DistanceTracker && DistanceWidget)
	{
		DistanceWidget->UpdateDistanceDisplay(DistanceTracker->DistanceMeters);
	}

	// Get the current camera boom location
	FVector CurrentBoomLocation = CameraBoom->GetComponentLocation();

	// Get the player's current location
	FVector PlayerLocation = GetActorLocation();

	// Calculate the new boom location
	// The X and Y coordinates are kept fixed, while the Z matches the player
	FVector NewBoomLocation;
	NewBoomLocation.X = CurrentBoomLocation.X;
	NewBoomLocation.Y = CurrentBoomLocation.Y;
	NewBoomLocation.Z = PlayerLocation.Z + 500.f;

	// Set the new location for the camera boom
	CameraBoom->SetWorldLocation(NewBoomLocation);
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
	if (GetCharacterMovement()->IsMovingOnGround() || TimeSinceLeftGround <= KoyoteTime) 
	{
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

void ARunnerCharacter::FallingGravity(float DeltaTime)
{
	bool bIsFalling = GetCharacterMovement()->IsFalling();

	if (!bIsFalling)
	{
		FallGravityMultiplier = FallGravityMultiplierMin;
		return;
	}

	FallGravityMultiplier = (FallGravityMultiplier < FallGravityMultiplierMax) ? FallGravityMultiplier + DeltaTime * FallGravityScaler : FallGravityMultiplierMax;
	FVector tempVel = GetCharacterMovement()->Velocity;
	tempVel.Z -= FallGravityMultiplier * DeltaTime;
	GetCharacterMovement()->Velocity = tempVel;
}

void ARunnerCharacter::RestartLevel()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()));
}

void ARunnerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr)
	{
		ASpikes* WallSpike = Cast<AWallSpikes>(OtherActor);
		ASpikes* Spike = Cast<ASpikes>(OtherActor);

		// Collided with one of the spikes and it is game over
		if (WallSpike || Spike)
		{
			GetMesh()->Deactivate();
			GetMesh()->SetVisibility(false);
			CanMove = false;

			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(UnusedHandle, this, &ARunnerCharacter::RestartLevel, 2.f, false);
		}
	}
}

