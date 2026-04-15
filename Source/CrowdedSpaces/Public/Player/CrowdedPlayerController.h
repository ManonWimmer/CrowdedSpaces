#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "Grid/GridActor.h"
#include "Player/PlayerActionsData.h"
#include "UI/GameHUD.h"
#include "CrowdedPlayerController.generated.h"

class UActionSubsystem;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCameraMoveForward, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCameraMoveRight, float, Value);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCameraRotate, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCameraMouseWheelClickRotate, FVector2D, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMouseMove, FVector2D, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCameraZoom, float, Value);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLeftClickBuild);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLeftClickGame);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRightClickBuild);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRightClickGame);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLeftRotateBuild);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRightRotateBuild);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeInputChanged, int, TimeIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTogglePause);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectableActorSelected, AActor*, SelectableActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNotSelectableActorSelected);

UCLASS()
class CROWDEDSPACES_API ACrowdedPlayerController : public APlayerController
{
	GENERATED_BODY()

	ACrowdedPlayerController();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputMappingContext> CameraIMC{nullptr};
	
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
	FOnCameraMouseWheelClickRotate OnCameraMouseWheelClickRotate;

	UPROPERTY(BlueprintAssignable)
	FOnMouseMove OnMouseMove;

	UPROPERTY(BlueprintAssignable)
	FOnCameraZoom OnCameraZoom;

	// Left click
	UPROPERTY(BlueprintAssignable)
	FOnLeftClickBuild OnLeftClickBuild;

	UPROPERTY(BlueprintAssignable)
	FOnLeftClickGame OnLeftClickGame;

	// Right click
	UPROPERTY(BlueprintAssignable)
	FOnRightClickBuild OnRightClickBuild;

	UPROPERTY(BlueprintAssignable)
	FOnRightClickBuild OnRightClickGame;

	// Rotate build
	UPROPERTY(BlueprintAssignable)
	FOnLeftRotateBuild OnLeftRotateBuild;

	UPROPERTY(BlueprintAssignable)
	FOnRightRotateBuild OnRightRotateBuild;

	// Time
	UPROPERTY(BlueprintAssignable)
	FOnTimeInputChanged OnTimeInputChanged;

	UPROPERTY(BlueprintAssignable)
	FOnTogglePause OnTogglePause;

	// Selection
	UPROPERTY(BlueprintAssignable)
	FOnSelectableActorSelected OnSelectableActorSelected;

	UPROPERTY(BlueprintAssignable)
	FOnNotSelectableActorSelected OnNotSelectableActorSelected;
	
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

	void MouseMoveInput(const FInputActionValue& Value);
	void StartMouseWheelRotate(const FInputActionValue& Value) { bIsRotatingCameraWithMouseWheel = true; }
	void StopMouseWheelRotate(const FInputActionValue& Value) { bIsRotatingCameraWithMouseWheel = false; }
	
	void ZoomInput(const FInputActionValue& Value) { OnCameraZoom.Broadcast(Value.Get<float>()); }

	void LeftClickInput(const FInputActionValue& Value);
	void RightClickInput(const FInputActionValue& Value);

	void LeftRotateBuildInput(const FInputActionValue& Value);
	void RightRotateBuildInput(const FInputActionValue& Value);

	void Time0Input(const FInputActionValue& Value) { OnTimeInputChanged.Broadcast(0); }
	void Time1Input(const FInputActionValue& Value) { OnTimeInputChanged.Broadcast(1); }
	void Time2Input(const FInputActionValue& Value) { OnTimeInputChanged.Broadcast(2); }
	void Time3Input(const FInputActionValue& Value) { OnTimeInputChanged.Broadcast(3); }
	void TogglePauseInput(const FInputActionValue& Value) { OnTogglePause.Broadcast(); }

	// Selection
	void HandleSelection() const;

	UPROPERTY()
	TObjectPtr<AGridActor> GridActor{nullptr};
	
	UPROPERTY()
	TObjectPtr<AActor> SelectedObject{nullptr};

	UPROPERTY()
	TObjectPtr<AGameHUD> GameHUD{nullptr};

	UPROPERTY()
	bool bIsRotatingCameraWithMouseWheel{nullptr};
};