#include "Action/ActionWidgetManager.h"

#include "Action/ActionComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "UI/Widgets/World/ActionWidget.h"
#include "Camera/FreeCameraPawn.h"
#include "Game/CrowdedGameState.h"

void UActionWidgetManager::Initialize(APlayerController* InPC)
{
	PC = InPC;

	if (!PC) return;

	const ACrowdedGameState* GameState = GetWorld()->GetGameState<ACrowdedGameState>();
	if (!GameState) return;
	
	WidgetClass = GameState->ActionWidgetClass;
}

void UActionWidgetManager::ShowForActor(AActor* Actor)
{
	if (!PC || !Actor || !WidgetClass) return;

	if (CurrentActor == Actor)
	{
		Hide();
		return;
	}
	
	CurrentActor = Actor;

	if (!Widget)
	{
		Widget = CreateWidget<UActionWidget>(PC, WidgetClass);
		Widget->AddToViewport(100);
		Widget->SetAlignmentInViewport(FVector2D(0.5f, 0.5f));
		Widget->SetAnchorsInViewport(FAnchors(0.5f, 0.5f));
	}

	Widget->SetVisibility(ESlateVisibility::Visible);

	Widget->SetupActions(Actor->GetComponentByClass<UActionComponent>()->GetAvailableActions(Actor), Actor);

	bIsVisible = true;
}

void UActionWidgetManager::Hide()
{
	if (Widget)
	{
		Widget->SetVisibility(ESlateVisibility::Hidden);
	}

	CurrentActor = nullptr;
	bIsVisible = false;
}

void UActionWidgetManager::Tick(float DeltaTime)
{
	if (!bIsVisible || !CurrentActor || !Widget || !PC)
		return;

	UpdateScreenPosition();
}

void UActionWidgetManager::UpdateScreenPosition() const
{
	FVector WorldPos = CurrentActor->GetActorLocation() + FVector(0, 0, 100.f);

	FVector2D ScreenPos;

	if (!PC->ProjectWorldLocationToScreen(WorldPos, ScreenPos))
		return;
	
	float UIScale = 1.f;

	// Scale with camera zoom
	if (const AFreeCameraPawn* Cam = Cast<AFreeCameraPawn>(PC->GetPawn()))
	{
		const float ZoomAlpha = Cam->GetZoomAlpha();
		
		UIScale = FMath::Lerp(1.f, .5f, ZoomAlpha);
	}
	
	Widget->SetPositionInViewport(ScreenPos, true);
	Widget->SetRenderScale(FVector2D(UIScale, UIScale));
}
