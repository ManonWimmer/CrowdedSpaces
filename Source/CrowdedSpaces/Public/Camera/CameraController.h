// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "Player/PlayerActionsData.h"
#include "EnhancedInputComponent.h"
#include "CameraController.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCameraMoveForward, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCameraMoveRight, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCameraRotate, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCameraZoom, float, Value);

UCLASS()
class CROWDEDSPACES_API ACameraController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputMappingContext* CameraIMC;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UPlayerActionsData> PlayerInputsData;

	// Delegates
	UPROPERTY(BlueprintAssignable)
	FOnCameraMoveForward OnCameraMoveForward;

	UPROPERTY(BlueprintAssignable)
	FOnCameraMoveRight OnCameraMoveRight;

	UPROPERTY(BlueprintAssignable)
	FOnCameraRotate OnCameraRotate;

	UPROPERTY(BlueprintAssignable)
	FOnCameraZoom OnCameraZoom;
	
protected:
	virtual void SetupInputComponent() override;

private:
	// Axis functions
	void MoveForwardInput(const FInputActionValue& Value) { OnCameraMoveForward.Broadcast(Value.Get<float>()); }
	void StopMoveForwardInput(const FInputActionValue& Value) { OnCameraMoveForward.Broadcast(0.f); }
	
	void MoveRightInput(const FInputActionValue& Value) { OnCameraMoveRight.Broadcast(Value.Get<float>()); }
	void StopMoveRightInput(const FInputActionValue& Value) { OnCameraMoveRight.Broadcast(0.f); }
	
	void RotateInput(const FInputActionValue& Value) { OnCameraRotate.Broadcast(Value.Get<float>()); }
	void StopRotateInput(const FInputActionValue& Value) { OnCameraRotate.Broadcast(0.f); }
	
	void ZoomInput(const FInputActionValue& Value) { OnCameraZoom.Broadcast(Value.Get<float>()); }
};
