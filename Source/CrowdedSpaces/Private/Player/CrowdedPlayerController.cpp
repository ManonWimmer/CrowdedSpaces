#include "Player/CrowdedPlayerController.h"

#include "EngineUtils.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Game/CrowdedGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Time/TimeSubsystem.h"
#include "UI/Widgets/SelectionWidget.h"

ACrowdedPlayerController::ACrowdedPlayerController(): CameraIMC(nullptr)
{
	bShouldPerformFullTickWhenPaused = true;
}

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
	EIC->BindAction(PlayerInputsData->RotateKeyAction, ETriggerEvent::Started, this, &ACrowdedPlayerController::RotateInput);
	EIC->BindAction(PlayerInputsData->RotateKeyAction, ETriggerEvent::Triggered, this, &ACrowdedPlayerController::RotateInput);
	EIC->BindAction(PlayerInputsData->RotateKeyAction, ETriggerEvent::Completed, this, &ACrowdedPlayerController::StopRotateInput);

	EIC->BindAction(PlayerInputsData->MouseMoveAction, ETriggerEvent::Triggered, this, &ACrowdedPlayerController::MouseMoveInput);
	EIC->BindAction(PlayerInputsData->RotateMouseWheelClickAction, ETriggerEvent::Started, this, &ACrowdedPlayerController::StartMouseWheelRotate);
	EIC->BindAction(PlayerInputsData->RotateMouseWheelClickAction, ETriggerEvent::Completed, this, &ACrowdedPlayerController::StopMouseWheelRotate);
	
	// Zoom
	EIC->BindAction(PlayerInputsData->ZoomAction, ETriggerEvent::Started, this, &ACrowdedPlayerController::ZoomInput);

	// Left & right click
	EIC->BindAction(PlayerInputsData->LeftClickAction, ETriggerEvent::Started, this, &ACrowdedPlayerController::LeftClickInput);
	EIC->BindAction(PlayerInputsData->RightClickAction, ETriggerEvent::Started, this, &ACrowdedPlayerController::RightClickInput);

	// Build rotate
	EIC->BindAction(PlayerInputsData->LeftRotateBuildAction, ETriggerEvent::Started, this, &ACrowdedPlayerController::LeftRotateBuildInput);
	EIC->BindAction(PlayerInputsData->RightRotateBuildAction, ETriggerEvent::Started, this, &ACrowdedPlayerController::RightRotateBuildInput);

	// Time
	EIC->BindAction(PlayerInputsData->Time0Action, ETriggerEvent::Started, this, &ACrowdedPlayerController::Time0Input);
	EIC->BindAction(PlayerInputsData->Time1Action, ETriggerEvent::Started, this, &ACrowdedPlayerController::Time1Input);
	EIC->BindAction(PlayerInputsData->Time2Action, ETriggerEvent::Started, this, &ACrowdedPlayerController::Time2Input);
	EIC->BindAction(PlayerInputsData->Time3Action, ETriggerEvent::Started, this, &ACrowdedPlayerController::Time3Input);
	EIC->BindAction(PlayerInputsData->TogglePauseAction, ETriggerEvent::Started, this, &ACrowdedPlayerController::TogglePauseInput);
	
	// Add IMC
	TObjectPtr<ULocalPlayer> LocalPlayer = GetLocalPlayer();
	if (!LocalPlayer)
		return;

	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
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

	// Time
	UWorld* World = GetWorld();
	if (!World)
		return;
	
	UTimeSubsystem* TimeSubsystem = World->GetSubsystem<UTimeSubsystem>();
	if (!TimeSubsystem)
		return;

	OnTimeInputChanged.AddDynamic(TimeSubsystem, &UTimeSubsystem::OnTimeInputChanged);
	OnTogglePause.AddDynamic(TimeSubsystem, &UTimeSubsystem::OnTogglePause);
}

void ACrowdedPlayerController::MouseMoveInput(const FInputActionValue& Value)
{
	if (!bIsRotatingCameraWithMouseWheel)
		return;

	FVector2D MouseDelta = Value.Get<FVector2D>();

	OnCameraMouseWheelClickRotate.Broadcast(MouseDelta);
}

void ACrowdedPlayerController::LeftClickInput(const FInputActionValue& Value)
{
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

void ACrowdedPlayerController::RightClickInput(const FInputActionValue& Value)
{
	const TObjectPtr<ACrowdedGameMode> GameMode = Cast<ACrowdedGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GameMode)
		return;
	
	if (GameMode->GetGameMode() == EGameModeState::Building)
	{
		OnRightClickBuild.Broadcast();
	}
	else if (GameMode->GetGameMode() == EGameModeState::Game)
	{
		OnRightClickGame.Broadcast();	
	}
}

void ACrowdedPlayerController::LeftRotateBuildInput(const FInputActionValue& Value)
{
	const TObjectPtr<ACrowdedGameMode> GameMode = Cast<ACrowdedGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GameMode)
		return;
	
	if (GameMode->GetGameMode() != EGameModeState::Building)
		return;

	OnLeftRotateBuild.Broadcast();
}

void ACrowdedPlayerController::RightRotateBuildInput(const FInputActionValue& Value)
{
	const TObjectPtr<ACrowdedGameMode> GameMode = Cast<ACrowdedGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
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
		const ISelectable* Selectable = Cast<ISelectable>(HitActor);
		GameHUD->ShowSelectionWidget(HitActor, true, Selectable->	GetSelectionType());
		return;
	}

	// 2. Room
	if (GridActor)
	{
		FGridRoom* Room;

		if (GridActor->GetRoomAtWorldLocation(Hit.Location, Room) && Room)
		{
			if (Room->RoomType == EGridRoomType::Storage)
			{
				GameHUD->ShowSelectionWidget(*Room, true, ESelectionType::StorageRoom);
			}
			else
			{
				GameHUD->ShowSelectionWidget(*Room, true, ESelectionType::Room);
			}
			
			return;
		}
	}

	// 3. Nothing
	GameHUD->ShowSelectionWidget(nullptr, false, ESelectionType::Default);
}
