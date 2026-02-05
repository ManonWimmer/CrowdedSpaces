#include "Build/BuildSubsystem.h"

#include "EngineUtils.h"
#include "Grid/GridActor.h"
#include "Game/CrowdedGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CrowdedPlayerController.h"
#include "Resources/MoneyComponent.h"
#include "Player/CrowdedPlayerState.h"

void UBuildSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	// Game Mode
	TObjectPtr<ACrowdedGameMode> GameMode = Cast<ACrowdedGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GameMode)
		return;
	
	GameMode->OnGameModeChanged.AddDynamic(this, &UBuildSubsystem::OnGameModeChanged);

	// Player controller
	TObjectPtr<APlayerController> PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC)
		return;

	TObjectPtr<ACrowdedPlayerController> CamPC = Cast<ACrowdedPlayerController>(PC);
	if (!CamPC)
		return;

	CamPC->OnLeftClickBuild.AddDynamic(this, &UBuildSubsystem::LeftClicked);

	// Money component
	if (TObjectPtr<ACrowdedPlayerState> PS = PC->GetPlayerState<ACrowdedPlayerState>())
	{
		MoneyComponent = PS->GetMoneyComponent();
	}

	// HUD
	GameHUD = Cast<AGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	// Grid actor
	for (TActorIterator<AGridActor> It(GetWorld()); It; ++It)
	{
		GridActor = *It;
		break; 
	}

	if (!GridActor)
		UE_LOG(LogTemp, Error, TEXT("BuildSubsystem: couldn't find GridActor in world"));
}

TStatId UBuildSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UBuildSubsystem, STATGROUP_Tickables); // Sinon crash quand Tickable true
}

void UBuildSubsystem::OnGameModeChanged(EGameModeState NewMode)
{
	// Activer ou désactiver le tick selon le mode
	bTickEnabled = (NewMode == EGameModeState::Building);

	StopBuilding();

	// UI
	if (NewMode == EGameModeState::Building)
	{
		if (GameHUD)
			GameHUD->ShowBuildWidget(true);
	}
	else
	{
		if (GameHUD)
			GameHUD->ShowBuildWidget(false);
	}
}

void UBuildSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateGhost();
}

bool UBuildSubsystem::IsTickable() const
{
	return bTickEnabled;
}

void UBuildSubsystem::StartBuilding(UBuildData* BuildData)
{
	if (!BuildData || !BuildData->BuildClass)
		return;

	bIsSelectingRoom = false;
	
	CurrentBuildData = BuildData;

	if (!CurrentGhost)
	{
		CurrentGhost = GetWorld()->SpawnActor<AGhostObject>(
			AGhostObject::StaticClass(),
			FVector::ZeroVector,
			FRotator::ZeroRotator
		);
	}

	CurrentGhost->SetActorHiddenInGame(false);

	// Get mesh from buildable
	TObjectPtr<ABuildableObject> const DefaultBuildable =
		BuildData->BuildClass->GetDefaultObject<ABuildableObject>();

	if (!DefaultBuildable)
		return;
	
	TObjectPtr<UStaticMesh> GhostMesh = DefaultBuildable->GetMeshComponent()->GetStaticMesh();
	CurrentGhost->SetMesh(GhostMesh);

	// Scale
	CurrentGhost->SetActorScale3D(DefaultBuildable->GetActorScale3D());
}

void UBuildSubsystem::StopBuilding()
{
	CurrentBuildData = nullptr;

	// Stop object selection
	if (CurrentGhost)
		CurrentGhost->SetActorHiddenInGame(true);

	if (GridActor)
		GridActor->DeselectSelectedCells();

	//todo: Stop room selection
}

void UBuildSubsystem::PlaceObject()
{
	if (!CurrentGhost || !CurrentBuildData || !CurrentBuildData->BuildClass || !GridActor)
		return;

	int SizeX = CurrentBuildData->GridRowsX;
	int SizeY = CurrentBuildData->GridColumnsY;

	int StartRow = 0, StartCol = 0;
	GridActor->GetCellAtLocation(CurrentGhost->GetActorLocation(), StartRow, StartCol);
	
	StartRow -= SizeX / 2;
	StartCol -= SizeY / 2;

	StartRow = FMath::Clamp(StartRow, 0, GridActor->GetRows() - SizeX);
	StartCol = FMath::Clamp(StartCol, 0, GridActor->GetColumns() - SizeY);

	// Check can place
	for (int Row = StartRow; Row < StartRow + SizeX; ++Row)
	{
		for (int Col = StartCol; Col < StartCol + SizeY; ++Col)
		{
			FGridCell* Cell = GridActor->GetGridCell(Row, Col);
			if (!Cell || Cell->bOccupied)
				return; 
		}
	}
	TObjectPtr<ABuildableObject> Placed = GetWorld()->SpawnActor<ABuildableObject>(
		CurrentBuildData->BuildClass,
		CurrentGhost->GetActorLocation(),
		FRotator::ZeroRotator
	);

	if (!Placed)
		return;

	// Default scale
	TObjectPtr<ABuildableObject> DefaultBuildable = CurrentBuildData->BuildClass->GetDefaultObject<ABuildableObject>();
	Placed->SetActorScale3D(DefaultBuildable->GetActorScale3D());

	// Set cells occupied
	for (int Row = StartRow; Row < StartRow + SizeX; ++Row)
	{
		for (int Col = StartCol; Col < StartCol + SizeY; ++Col)
		{
			FGridCell* Cell = GridActor->GetGridCell(Row, Col);
			if (Cell)
				Cell->bOccupied = true;
		}
	}

	if (MoneyComponent)
		MoneyComponent->RemoveMoney(CurrentBuildData->MoneyCost);
}


void UBuildSubsystem::UpdateGhost() const
{
	if(!CurrentGhost || !GridActor)
		return;

	if (CurrentGhost->IsHidden())
		return;

	FVector HitLocation;
	if(!GetCursorHit(HitLocation))
	{
		return;
	}

	int HitRow, HitCol;
	if (!GridActor->GetCellAtLocation(HitLocation, HitRow, HitCol))
	{
		GridActor->DeselectSelectedCells();
		return;
	}

	if (!CurrentBuildData)
		return;

	int SizeX = CurrentBuildData->GridRowsX;
	int SizeY = CurrentBuildData->GridColumnsY;

	int StartRow = HitRow - (SizeX - 1) / 2;
	int StartCol = HitCol - (SizeY - 1) / 2;

	StartRow = FMath::Clamp(StartRow, 0, GridActor->GetRows() - SizeX);
	StartCol = FMath::Clamp(StartCol, 0, GridActor->GetColumns() - SizeY);
	
	GridActor->DeselectSelectedCells();
	for (int Row = StartRow; Row < StartRow + SizeX; ++Row)
	{
		for (int Col = StartCol; Col < StartCol + SizeY; ++Col)
		{
			FGridCell* Cell = GridActor->GetGridCell(Row, Col);
			if (Cell)
				GridActor->SelectObjectCell(Row, Col);
		}
	}
	
	FVector2D TopLeft;
	GridActor->GetGridLocation(false, StartRow, StartCol, TopLeft);
	
	FVector SnappedLocation(
		TopLeft.X + (SizeX * GridActor->GetCellSize()) / 2.0f,
		TopLeft.Y + (SizeY * GridActor->GetCellSize()) / 2.0f,
		GridActor->GetActorLocation().Z
	);

	CurrentGhost->SetActorLocation(SnappedLocation);
}

void UBuildSubsystem::StartRoomSelection(EGridRoomType RoomType)
{
	CurrentRoomType = RoomType;
	bIsSelectingRoom = true;

	SelectedRoomCells.Empty();

	if (CurrentGhost)
		CurrentGhost->SetActorHiddenInGame(true);
}

void UBuildSubsystem::LeftClicked()
{
	if (bIsSelectingRoom)
	{
		FVector HitLocation;
		if(!GetCursorHit(HitLocation))
		{
			GridActor->DeselectSelectedCells();
			return;
		}

		int HitRow, HitCol;
		if (!GridActor->GetCellAtLocation(HitLocation, HitRow, HitCol))
		{
			GridActor->DeselectSelectedCells();
			return;
		}
		ToggleRoomCell(HitRow, HitCol);
	}
	else
	{
		PlaceObject();
	}
}

void UBuildSubsystem::ToggleRoomCell(int Row, int Column)
{
	if (!bIsSelectingRoom)
		return;
	
	FGridCell* Cell = GridActor->GetGridCell(Row, Column);

	if (!Cell || Cell->CellType == EGridCellType::Wall)
		return;

	if (SelectedRoomCells.Contains(Cell))
	{
		SelectedRoomCells.Remove(Cell);
		GridActor->DeselectCell(Row, Column);
	}
	else
	{
		SelectedRoomCells.Add(Cell);
		GridActor->SelectRoomCell(Row, Column);
	}
}

void UBuildSubsystem::ConfirmRoom()
{
	if (SelectedRoomCells.Num() == 0 || !bIsSelectingRoom)
		return;

	//todo: check min size, is in object

	int RoomId = GridActor->CreateRoom(CurrentRoomType, SelectedRoomCells);

	SelectedRoomCells.Empty();
	GridActor->DeselectSelectedCells();
	bIsSelectingRoom = false;
}

bool UBuildSubsystem::GetCursorHit(FVector& OutHit) const
{
	TObjectPtr<APlayerController> PC = GetWorld()->GetFirstPlayerController();
	if (!PC)
		return false;
	
	float MouseX, MouseY;
	if(PC->GetMousePosition(MouseX, MouseY))
	{
		FVector WorldOrigin, WorldDir;
		if(PC->DeprojectScreenPositionToWorld(MouseX, MouseY, WorldOrigin, WorldDir))
		{
			FHitResult Hit;
			if(GetWorld()->LineTraceSingleByChannel(Hit, WorldOrigin, WorldOrigin + WorldDir * CursorLineTraceDistance, ECC_Visibility))
			{
				OutHit = Hit.Location;
				return true;
			}
		}
	}
	return false;
}
