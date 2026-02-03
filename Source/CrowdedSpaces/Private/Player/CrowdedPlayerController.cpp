#include "Player/CrowdedPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Game/CrowdedGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Widgets/SelectionWidget.h"

void ACrowdedPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	TObjectPtr<UEnhancedInputComponent> EIC = Cast<UEnhancedInputComponent>(InputComponent);
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
	TObjectPtr<ULocalPlayer> LP = GetLocalPlayer();
	if (!LP)
		return;

	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> InputSubsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
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
	//if (GEngine)
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, "Left click event");
	
	TObjectPtr<ACrowdedGameMode> GameMode = Cast<ACrowdedGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GameMode)
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
		SelectedObject = Cast<ISelectable>(Hit.GetActor());
		SelectedObject->OnSelected();

		GameHUD->ShowSelectionWidget(true);

		if (GameHUD->GetSelectionWidget())
		{
			// Bind automatique à toutes les stats du composant
			GameHUD->GetSelectionWidget()->BindToSelectable(Hit.GetActor(), SelectedObject->GetDisplayName(), SelectedObject->SelectionType);
		}
	}
}
