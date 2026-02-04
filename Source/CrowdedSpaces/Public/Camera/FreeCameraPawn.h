#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FreeCameraPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class CROWDEDSPACES_API AFreeCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	AFreeCameraPawn();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	// Components
	UPROPERTY(VisibleAnywhere, Category="Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, Category="Camera")
	TObjectPtr<UCameraComponent> Camera;
	
	// Movement
	UPROPERTY(EditAnywhere, Category="Movement")
	float MoveSpeed = 1500.f;

	UPROPERTY(EditAnywhere, Category="Movement")
	float MoveSmooth = 50.f;
	
	UPROPERTY(EditAnywhere, Category="Rotation")
	float RotationSpeed = 60.f;

	UPROPERTY(EditAnywhere, Category="Rotation")
	float RotationSmooth = 50.f;

	UPROPERTY(EditAnywhere, Category="Zoom")
	float ZoomSpeed = 500.f;

	UPROPERTY(EditAnywhere, Category="Zoom")
	float MinZoom = 800.f;

	UPROPERTY(EditAnywhere, Category="Zoom")
	float MaxZoom = 3000.f;

	UPROPERTY(EditAnywhere, Category="Zoom")
	float ZoomSmooth = 50.f;

	UPROPERTY(EditAnywhere, Category="Movement")
	FVector2D MapLimitsX = FVector2D(-5000.f, 5000.f);

	UPROPERTY(EditAnywhere, Category="Movement")
	FVector2D MapLimitsY = FVector2D(-5000.f, 5000.f);

private:
	FVector CurrentVelocity = FVector::ZeroVector;
	float TargetZoom = 0.f;
	float CurrentYawInput = 0.f;

	// Bind controller delegates
	void BindControllerEvents();
	
	// Delegate handlers
	UFUNCTION()
	void OnMoveForward(float Value);
	
	UFUNCTION()
	void OnMoveRight(float Value);
	
	UFUNCTION()
	void OnRotate(float Value);
	
	UFUNCTION()
	void OnZoom(float Value);

	// Movement / Rotation
	void ApplyMovement(float DeltaTime);
	void ApplyRotation(float DeltaTime);	
};
