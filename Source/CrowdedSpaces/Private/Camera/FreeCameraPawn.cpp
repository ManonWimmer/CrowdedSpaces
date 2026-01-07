// Fill out your copyright notice in the Description page of Project Settings.


#include "CrowdedSpaces/Public/Camera/FreeCameraPawn.h"

#include "Camera/CameraComponent.h"
#include "Camera/CameraController.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
AFreeCameraPawn::AFreeCameraPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// Spring Arm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bDoCollisionTest = false;
	SpringArm->TargetArmLength = 1500.f;
	SpringArm->SetRelativeRotation(FRotator(-55.f, 0.f, 0.f)); // 3/4 Angle

	// Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	// Default zoom
	TargetZoom = SpringArm->TargetArmLength;

	AutoPossessPlayer = EAutoReceiveInput::Player0; 
}

// Called when the game starts or when spawned
void AFreeCameraPawn::BeginPlay()
{
	Super::BeginPlay();
	BindControllerEvents();
}

// Called every frame
void AFreeCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ApplyMovement(DeltaTime);
	ApplyRotation(DeltaTime);

	// Smooth zoom
	SpringArm->TargetArmLength = FMath::FInterpTo(
	SpringArm->TargetArmLength,
	TargetZoom,
	DeltaTime,
	ZoomSmooth
	);
}

void AFreeCameraPawn::BindControllerEvents()
{
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (ACameraController* CamPC = Cast<ACameraController>(PC))
		{
			CamPC->OnCameraMoveForward.AddDynamic(this, &AFreeCameraPawn::OnMoveForward);
			CamPC->OnCameraMoveRight.AddDynamic(this, &AFreeCameraPawn::OnMoveRight);
			CamPC->OnCameraRotate.AddDynamic(this, &AFreeCameraPawn::OnRotate);
			CamPC->OnCameraZoom.AddDynamic(this, &AFreeCameraPawn::OnZoom);
		}
	}
}

void AFreeCameraPawn::OnMoveForward(float Value)
{
	CurrentVelocity.X = Value * MoveSpeed;
}

void AFreeCameraPawn::OnMoveRight(float Value)
{
	CurrentVelocity.Y = Value * MoveSpeed;
}

void AFreeCameraPawn::OnRotate(float Value)
{
	CurrentYawInput = Value * RotationSpeed;
}

void AFreeCameraPawn::OnZoom(float Value)
{
	TargetZoom = FMath::Clamp(
		TargetZoom - Value * ZoomSpeed,
		MinZoom,
		MaxZoom
	);
}

void AFreeCameraPawn::ApplyMovement(float DeltaTime)
{
	if (!CurrentVelocity.IsNearlyZero())
	{
		FVector TargetLocation = GetActorLocation();

		if (!CurrentVelocity.IsNearlyZero())
		{
			FVector Forward = GetActorForwardVector();
			FVector Right = GetActorRightVector();

			TargetLocation += (Forward * CurrentVelocity.X + Right * CurrentVelocity.Y) * DeltaTime;
		}

		// Clamp
		TargetLocation.X = FMath::Clamp(TargetLocation.X, MapLimitsX.X, MapLimitsX.Y);
		TargetLocation.Y = FMath::Clamp(TargetLocation.Y, MapLimitsY.X, MapLimitsY.Y);
		
		// Smooth
		FVector Smoothed = FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, MoveSmooth);
		
		SetActorLocation(Smoothed);
	}
}

void AFreeCameraPawn::ApplyRotation(float DeltaTime)
{
	float TargetYaw = GetActorRotation().Yaw + CurrentYawInput * DeltaTime;
	
	float SmoothedYaw = FMath::FInterpTo(
		GetActorRotation().Yaw,
		TargetYaw,
		DeltaTime,
		RotationSmooth
	);

	SetActorRotation(FRotator(GetActorRotation().Pitch, SmoothedYaw, 0.f));
}

