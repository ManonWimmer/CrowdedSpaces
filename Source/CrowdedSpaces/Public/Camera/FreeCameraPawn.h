#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "FreeCameraPawn.generated.h"

class UCameraComponent;

UCLASS()
class CROWDEDSPACES_API AFreeCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	AFreeCameraPawn();

	UFUNCTION()
	UCameraComponent* GetCameraComponent() const { return Camera; }

	float GetCurrentZoom() const { return SpringArm->TargetArmLength; }
	float GetDefaultZoom() const { return 1500.f; }

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

	UPROPERTY(EditDefaultsOnly)
	float DefaultZoom = 1500.f;

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
	float MouseYawInput = 0.f;
	
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
	void OnMouseWheelRotate(FVector2D MouseDelta);
	
	UFUNCTION()
	void OnZoom(float Value);

	// Movement / Rotation
	void ApplyMovement(float DeltaTime);
	void ApplyRotation(float DeltaTime);
	
};
