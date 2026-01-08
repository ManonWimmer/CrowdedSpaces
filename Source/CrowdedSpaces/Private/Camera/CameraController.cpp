#include "Camera/CameraController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Game/GameModeSubsystem.h"

void ACameraController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (!PlayerInputsData) return;

		// Move Forward
		EIC->BindAction(PlayerInputsData->MoveForwardAction, ETriggerEvent::Started, this, &ACameraController::MoveForwardInput);
		EIC->BindAction(PlayerInputsData->MoveForwardAction, ETriggerEvent::Triggered, this, &ACameraController::MoveForwardInput);
		EIC->BindAction(PlayerInputsData->MoveForwardAction, ETriggerEvent::Completed, this, &ACameraController::StopMoveForwardInput);

		// Move Right
		EIC->BindAction(PlayerInputsData->MoveRightAction, ETriggerEvent::Started, this, &ACameraController::MoveRightInput);
		EIC->BindAction(PlayerInputsData->MoveRightAction, ETriggerEvent::Triggered, this, &ACameraController::MoveRightInput);
		EIC->BindAction(PlayerInputsData->MoveRightAction, ETriggerEvent::Completed, this, &ACameraController::StopMoveRightInput);

		// Rotate
		EIC->BindAction(PlayerInputsData->RotateAction, ETriggerEvent::Started, this, &ACameraController::RotateInput);
		EIC->BindAction(PlayerInputsData->RotateAction, ETriggerEvent::Triggered, this, &ACameraController::RotateInput);
		EIC->BindAction(PlayerInputsData->RotateAction, ETriggerEvent::Completed, this, &ACameraController::StopRotateInput);

		// Zoom
		EIC->BindAction(PlayerInputsData->ZoomAction, ETriggerEvent::Started, this, &ACameraController::ZoomInput);

		// Left click
		EIC->BindAction(PlayerInputsData->LeftClickAction, ETriggerEvent::Started, this, &ACameraController::LeftClickInput);
	}

	// Add IMC
	if (ULocalPlayer* LP = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsystem->AddMappingContext(CameraIMC, 0);
		}
	}
}

void ACameraController::BeginPlay()
{
	Super::BeginPlay();

	// Cursor
	SetShowMouseCursor(true);
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void ACameraController::LeftClickInput(const FInputActionValue& Value)
{
	if (UGameModeSubsystem* Mode = GetWorld()->GetSubsystem<UGameModeSubsystem>())
	{
		if (Mode->GetGameMode() == EGameModeState::Building)
		{
			OnLeftClickBuild.Broadcast();
		}
		else if (Mode->GetGameMode() == EGameModeState::Game)
		{
			OnLeftClickGame.Broadcast();	
		}
	}
}
