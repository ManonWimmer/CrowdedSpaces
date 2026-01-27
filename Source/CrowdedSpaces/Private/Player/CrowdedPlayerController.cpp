#include "Player/CrowdedPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Game/CrowdedGameMode.h"
#include "Kismet/GameplayStatics.h"

void ACrowdedPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);
	if (!EIC)
		return;
	
	if (!PlayerInputsData) return;

	// Move Forward
	EIC->BindAction(PlayerInputsData->MoveForwardAction, ETriggerEvent::Started, this, &ACrowdedPlayerController::MoveForwardInput);
	EIC->BindAction(PlayerInputsData->MoveForwardAction, ETriggerEvent::Triggered, this, &ACrowdedPlayerController::MoveForwardInput);
	EIC->BindAction(PlayerInputsData->MoveForwardAction, ETriggerEvent::Completed, this, &ACrowdedPlayerController::StopMoveForwardInput);

	// Move Right
	EIC->BindAction(PlayerInputsData->MoveRightAction, ETriggerEvent::Started, this, &ACrowdedPlayerController::MoveRightInput);
	EIC->BindAction(PlayerInputsData->MoveRightAction, ETriggerEvent::Triggered, this, &ACrowdedPlayerController::MoveRightInput);
	EIC->BindAction(PlayerInputsData->MoveRightAction, ETriggerEvent::Completed, this, &ACrowdedPlayerController::StopMoveRightInput);

	// Rotate
	EIC->BindAction(PlayerInputsData->RotateAction, ETriggerEvent::Started, this, &ACrowdedPlayerController::RotateInput);
	EIC->BindAction(PlayerInputsData->RotateAction, ETriggerEvent::Triggered, this, &ACrowdedPlayerController::RotateInput);
	EIC->BindAction(PlayerInputsData->RotateAction, ETriggerEvent::Completed, this, &ACrowdedPlayerController::StopRotateInput);

	// Zoom
	EIC->BindAction(PlayerInputsData->ZoomAction, ETriggerEvent::Started, this, &ACrowdedPlayerController::ZoomInput);

	// Left click
	EIC->BindAction(PlayerInputsData->LeftClickAction, ETriggerEvent::Started, this, &ACrowdedPlayerController::LeftClickInput);

	// Add IMC
	ULocalPlayer* LP = GetLocalPlayer();
	if (!LP)
		return;

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!InputSubsystem)
		return;
	
	InputSubsystem->AddMappingContext(CameraIMC, 0);
}

void ACrowdedPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Cursor
	SetShowMouseCursor(true);
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	// Input mode à tester pour click ui
	SetInputMode(FInputModeGameAndUI());

	// Get HUD
	GameHUD = Cast<AGameHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
}

void ACrowdedPlayerController::LeftClickInput(const FInputActionValue& Value)
{
	ACrowdedGameMode* GameMode = Cast<ACrowdedGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GameMode)
		return;

	// Check click on UI
	if (!GameHUD)
		return;

	if (GameHUD->IsCursorHoveringUI())
		return;
	
	if (GameMode->GetGameMode() == EGameModeState::Building)
	{
		OnLeftClickBuild.Broadcast();
	}
	else if (GameMode->GetGameMode() == EGameModeState::Game)
	{
		OnLeftClickGame.Broadcast();	
	}

	if (!GameHUD)
		return;
	
	// Handle click selection
	FHitResult Hit;
	bool bHit = GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	
	if (SelectedObject)
	{
		SelectedObject->OnDeselected();
		SelectedObject = nullptr;
		GameHUD->ShowSelectionWidget(false);
		GameHUD->GetSelectionWidget()->Unbind();
	}

	if (Hit.GetActor() && Hit.GetActor()->Implements<USelectable>())
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Click on selectable");

		SelectedObject = Cast<ISelectable>(Hit.GetActor());
		SelectedObject->OnSelected();

		GameHUD->ShowSelectionWidget(true);

		if (GameHUD->GetSelectionWidget())
		{
			// Bind automatique à toutes les stats du composant
			GameHUD->GetSelectionWidget()->BindToSelectable(Hit.GetActor(), SelectedObject->GetDisplayName());
		}
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Failed click");
	}
}
