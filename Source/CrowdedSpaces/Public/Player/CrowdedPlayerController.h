#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "Grid/GridActor.h"
#include "Player/PlayerActionsData.h"
#include "UI/GameHUD.h"
#include "CrowdedPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCameraMoveForward, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCameraMoveRight, float, Value);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCameraRotate, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCameraZoom, float, Value);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLeftClickBuild);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLeftClickGame);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRightClickBuild);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRightClickGame);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLeftRotateBuild);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRightRotateBuild);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTime0);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTime1);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTime2);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTime3);

UCLASS()
class CROWDEDSPACES_API ACrowdedPlayerController : public APlayerController
{
	GENERATED_BODY()

	ACrowdedPlayerController();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputMappingContext* CameraIMC = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UPlayerActionsData> PlayerInputsData;

	// Camera move
	UPROPERTY(BlueprintAssignable)
	FOnCameraMoveForward OnCameraMoveForward;

	UPROPERTY(BlueprintAssignable)
	FOnCameraMoveRight OnCameraMoveRight;

	// Camera rotate & zoom
	UPROPERTY(BlueprintAssignable)
	FOnCameraRotate OnCameraRotate;

	UPROPERTY(BlueprintAssignable)
	FOnCameraZoom OnCameraZoom;

	// Left click
	UPROPERTY(BlueprintAssignable)
	FOnLeftClickBuild OnLeftClickBuild;

	UPROPERTY(BlueprintAssignable)
	FOnLeftClickGame OnLeftClickGame;

	// Right click
	UPROPERTY(BlueprintAssignable)
	FOnLeftClickBuild OnRightClickBuild;

	UPROPERTY(BlueprintAssignable)
	FOnLeftClickBuild OnRightClickGame;

	// Rotate
	UPROPERTY(BlueprintAssignable)
	FOnLeftRotateBuild OnLeftRotateBuild;

	UPROPERTY(BlueprintAssignable)
	FOnRightRotateBuild OnRightRotateBuild;

	// Time
	UPROPERTY(BlueprintAssignable)
	FOnTime0 OnTime0;

	UPROPERTY(BlueprintAssignable)
	FOnTime1 OnTime1;

	UPROPERTY(BlueprintAssignable)
	FOnTime2 OnTime2;

	UPROPERTY(BlueprintAssignable)
	FOnTime3 OnTime3;
	
protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

private:
	// Axis functions
	void MoveForwardInput(const FInputActionValue& Value) { OnCameraMoveForward.Broadcast(Value.Get<float>()); }
	void StopMoveForwardInput(const FInputActionValue& Value) { OnCameraMoveForward.Broadcast(0.f); }
	
	void MoveRightInput(const FInputActionValue& Value) { OnCameraMoveRight.Broadcast(Value.Get<float>()); }
	void StopMoveRightInput(const FInputActionValue& Value) { OnCameraMoveRight.Broadcast(0.f); }
	
	void RotateInput(const FInputActionValue& Value) { OnCameraRotate.Broadcast(Value.Get<float>()); }
	void StopRotateInput(const FInputActionValue& Value) { OnCameraRotate.Broadcast(0.f); }
	
	void ZoomInput(const FInputActionValue& Value) { OnCameraZoom.Broadcast(Value.Get<float>()); }

	void LeftClickInput(const FInputActionValue& Value);
	void RightClickInput(const FInputActionValue& Value);

	void LeftRotateBuildInput(const FInputActionValue& Value);
	void RightRotateBuildInput(const FInputActionValue& Value);

	void Time0Input(const FInputActionValue& Value) { OnTime0.Broadcast(); }
	void Time1Input(const FInputActionValue& Value) { OnTime1.Broadcast(); }
	void Time2Input(const FInputActionValue& Value) { OnTime2.Broadcast(); }
	void Time3Input(const FInputActionValue& Value) { OnTime3.Broadcast(); }

	// Selection
	void HandleSelection() const;

	UPROPERTY()
	AGridActor* GridActor = nullptr;
	
	UPROPERTY()
	AActor* SelectedObject = nullptr;

	UPROPERTY()
	TObjectPtr<AGameHUD> GameHUD = nullptr;
};