#pragma once

#include "CoreMinimal.h"

class UUserWidget;
class APlayerController;

class CROWDEDSPACES_API FUIUtils
{
public:
	static void UpdateWidgetScreenPosition(const APlayerController* PC, UUserWidget* Widget, const FVector& WorldLocation, float HeightOffset = 100.f);
	static void RotateComponentToCameraYaw(APlayerController* PC, USceneComponent* Component);
};