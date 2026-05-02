#include "UI/UIUtils.h"

#include "Blueprint/UserWidget.h"
#include "Camera/FreeCameraPawn.h"
#include "Kismet/KismetMathLibrary.h"

void FUIUtils::UpdateWidgetScreenPosition(const APlayerController* PC, UUserWidget* Widget, const FVector& WorldLocation, const float HeightOffset)
{
	if (!PC || !Widget)
		return;

	FVector WorldPos = WorldLocation + FVector(0, 0, HeightOffset);

	FVector2D ScreenPos;
	if (!PC->ProjectWorldLocationToScreen(WorldPos, ScreenPos))
		return;

	float UIScale = 1.f;

	// Zoom scaling
	if (const AFreeCameraPawn* Cam = Cast<AFreeCameraPawn>(PC->GetPawn()))
	{
		const float ZoomAlpha = Cam->GetZoomAlpha();
		UIScale = FMath::Lerp(1.f, 0.5f, ZoomAlpha);
	}

	Widget->SetPositionInViewport(ScreenPos, true);
	Widget->SetRenderScale(FVector2D(UIScale, UIScale));
}

void FUIUtils::RotateComponentToCameraYaw(APlayerController* PC, USceneComponent* Component)
{
	if (!PC || !Component)
		return;

	const APlayerCameraManager* CamManager = PC->PlayerCameraManager;
	if (!CamManager)
		return;

	const FVector CameraLocation = CamManager->GetCameraLocation();
	const FVector ComponentLocation = Component->GetComponentLocation();

	const FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(ComponentLocation, CameraLocation);
	
	const FRotator YawOnly(0.f, LookAt.Yaw, 0.f);

	Component->SetWorldRotation(YawOnly);
}
