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
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLeftRotateBuild);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRightRotateBuild);

UCLASS()
class CROWDEDSPACES_API ACrowdedPlayerController : public APlayerController
{
	GENERATED_BODY()

	ACrowdedPlayerController();

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

	UPROPERTY(BlueprintAssignable)
	FOnLeftClickBuild OnLeftClickBuild;

	UPROPERTY(BlueprintAssignable)
	FOnLeftClickGame OnLeftClickGame;

	UPROPERTY(BlueprintAssignable)
	FOnLeftRotateBuild OnLeftRotateBuild;

	UPROPERTY(BlueprintAssignable)
	FOnRightRotateBuild OnRightRotateBuild;
	
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

	void LeftRotateBuildInput(const FInputActionValue& Value);
	void RightRotateBuildInput(const FInputActionValue& Value);

	// Selection
	void HandleSelection() const;

	UPROPERTY()
	AGridActor* GridActor = nullptr;
	
	UPROPERTY()
	AActor* SelectedObject = nullptr;

	UPROPERTY()
	TObjectPtr<AGameHUD> GameHUD = nullptr;
};