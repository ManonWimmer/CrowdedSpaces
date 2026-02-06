#include "Player/CrowdedPlayerController.h"

#include "EngineUtils.h"
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

	// Camera rotate
	EIC->BindAction(PlayerInputsData->RotateAction, ETriggerEvent::Started, this, &ACrowdedPlayerController::RotateInput);
	EIC->BindAction(PlayerInputsData->RotateAction, ETriggerEvent::Triggered, this, &ACrowdedPlayerController::RotateInput);
	EIC->BindAction(PlayerInputsData->RotateAction, ETriggerEvent::Completed, this, &ACrowdedPlayerController::StopRotateInput);

	// Zoom
	EIC->BindAction(PlayerInputsData->ZoomAction, ETriggerEvent::Started, this, &ACrowdedPlayerController::ZoomInput);

	// Left click
	EIC->BindAction(PlayerInputsData->LeftClickAction, ETriggerEvent::Started, this, &ACrowdedPlayerController::LeftClickInput);

	// Build rotate
	EIC->BindAction(PlayerInputsData->LeftRotateBuildAction, ETriggerEvent::Started, this, &ACrowdedPlayerController::LeftRotateBuildInput);
	EIC->BindAction(PlayerInputsData->RightRotateBuildAction, ETriggerEvent::Started, this, &ACrowdedPlayerController::RightRotateBuildInput);
	
	// Add IMC
	TObjectPtr<ULocalPlayer> LP = GetLocalPlayer();
	if (!LP)
		return;

	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> InputSubsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!InputSubsystem)
		return;
	
	InputSubsystem->AddMappingContext(CameraIMC, 0);

	// Grid actor (selection room)
	for (TActorIterator<AGridActor> It(GetWorld()); It; ++It)
	{
		GridActor = *It;
		break;
	}
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
	
	HandleSelection();
}

void ACrowdedPlayerController::LeftRotateBuildInput(const FInputActionValue& Value)
{
	TObjectPtr<ACrowdedGameMode> GameMode = Cast<ACrowdedGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GameMode)
		return;
	
	if (GameMode->GetGameMode() != EGameModeState::Building)
		return;

	OnLeftRotateBuild.Broadcast();
}

void ACrowdedPlayerController::RightRotateBuildInput(const FInputActionValue& Value)
{
	TObjectPtr<ACrowdedGameMode> GameMode = Cast<ACrowdedGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GameMode)
		return;
	
	if (GameMode->GetGameMode() != EGameModeState::Building)
		return;

	OnRightRotateBuild.Broadcast();
}

void ACrowdedPlayerController::HandleSelection() const
{
	FHitResult Hit;
	bool bHit = GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	
	if (!GameHUD)
		return;

	AActor* HitActor = Hit.GetActor();

	// 1. Selectable actor (NPC, generator)
	if (HitActor && HitActor->Implements<USelectable>())
	{
		ISelectable* Selectable = Cast<ISelectable>(HitActor);
		GameHUD->ShowSelectionWidget(HitActor, true, Selectable->GetSelectionType());
		return;
	}

	// 2. Room
	if (GridActor)
	{
		FGridRoom* Room;

		if (GridActor->GetRoomAtWorldLocation(Hit.Location, Room))
		{
			GameHUD->ShowSelectionWidget(GridActor, true, ESelectionType::Room);
			return;
		}
	}

	// 3. Nothing
	GameHUD->ShowSelectionWidget(nullptr, false, ESelectionType::Default);
}
