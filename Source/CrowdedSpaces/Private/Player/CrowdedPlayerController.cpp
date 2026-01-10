#include "Player/CrowdedPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Game/GameModeSubsystem.h"

void ACrowdedPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
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

void ACrowdedPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Cursor
	SetShowMouseCursor(true);
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void ACrowdedPlayerController::LeftClickInput(const FInputActionValue& Value)
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
