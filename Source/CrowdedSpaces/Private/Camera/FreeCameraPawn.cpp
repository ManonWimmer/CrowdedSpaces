#include "CrowdedSpaces/Public/Camera/FreeCameraPawn.h"
#include "Camera/CameraComponent.h"
#include "Player/CrowdedPlayerController.h"
#include "Kismet/GameplayStatics.h"


AFreeCameraPawn::AFreeCameraPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// Spring Arm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bDoCollisionTest = false;
	SpringArm->TargetArmLength = DefaultZoom;
	SpringArm->SetRelativeRotation(FRotator(-55.f, 0.f, 0.f)); // 3/4 Angle

	// Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	// Default zoom
	TargetZoom = SpringArm->TargetArmLength;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	PrimaryActorTick.bTickEvenWhenPaused = true;
}

float AFreeCameraPawn::GetZoomAlpha() const
{
	return FMath::GetMappedRangeValueClamped(
		FVector2D(MinZoom, MaxZoom),
		FVector2D(0.f, 1.f),
		SpringArm->TargetArmLength
	);
}

void AFreeCameraPawn::BeginPlay()
{
	Super::BeginPlay();
	BindControllerEvents();
}

void AFreeCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float RealDeltaTime = FApp::GetDeltaTime();
	
	ApplyMovement(RealDeltaTime);
	ApplyRotation(RealDeltaTime);

	// Smooth zoom
	SpringArm->TargetArmLength = FMath::FInterpTo(
	SpringArm->TargetArmLength,
	TargetZoom,
	RealDeltaTime,
	ZoomSmooth
	);
}

void AFreeCameraPawn::BindControllerEvents()
{
	TObjectPtr<APlayerController> PC = Cast<APlayerController>(GetController());
	if (!PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("Free Camera Pawn : Player Controller Null"));
		return;
	}

	TObjectPtr<ACrowdedPlayerController> CrowdedPC = Cast<ACrowdedPlayerController>(PC);
	if (!CrowdedPC)
	{
		UE_LOG(LogTemp, Warning, TEXT("Free Camera Pawn : Crowded Player Controller Null"));
		return;
	}
	
	CrowdedPC->OnCameraMoveForward.AddDynamic(this, &AFreeCameraPawn::OnMoveForward);
	CrowdedPC->OnCameraMoveRight.AddDynamic(this, &AFreeCameraPawn::OnMoveRight);
	CrowdedPC->OnCameraRotate.AddDynamic(this, &AFreeCameraPawn::OnRotate);
	CrowdedPC->OnCameraMouseWheelClickRotate.AddDynamic(this, &AFreeCameraPawn::OnMouseWheelRotate);
	CrowdedPC->OnCameraZoom.AddDynamic(this, &AFreeCameraPawn::OnZoom);
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
	CurrentYawInput = -Value * RotationSpeed;
}

void AFreeCameraPawn::OnMouseWheelRotate(FVector2D MouseDelta)
{
	MouseYawInput = -MouseDelta.X * RotationSpeed;
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
	if (CurrentVelocity.IsNearlyZero())
		return;
	
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

void AFreeCameraPawn::ApplyRotation(float DeltaTime)
{
	float TotalYawInput = CurrentYawInput + MouseYawInput;

	float TargetYaw = GetActorRotation().Yaw + TotalYawInput * DeltaTime;
	
	float SmoothedYaw = FMath::FInterpTo(
		GetActorRotation().Yaw,
		TargetYaw,
		DeltaTime,
		RotationSmooth
	);

	SetActorRotation(FRotator(GetActorRotation().Pitch, SmoothedYaw, 0.f));
	
	MouseYawInput = 0.f;
}

