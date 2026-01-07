// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraController.h"
#include "EnhancedInputSubsystems.h"

void ACameraController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (!PlayerInputsData) return;

		// Bind movement
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
		
		EIC->BindAction(PlayerInputsData->ZoomAction, ETriggerEvent::Triggered, this, &ACameraController::ZoomInput);
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
