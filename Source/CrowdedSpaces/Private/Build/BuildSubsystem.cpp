#include "Build/BuildSubsystem.h"

#include "EngineUtils.h"
#include "Build/BuildableRegistrySubsystem.h"
#include "Build/GhostObject.h"
#include "Debug/CrowdedSpacesLogs.h"
#include "Grid/GridActor.h"
#include "Game/CrowdedGameMode.h"
#include "Game/CrowdedGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CrowdedPlayerController.h"
#include "Resources/ResourceComponent.h"
#include "Player/CrowdedPlayerState.h"

void UBuildSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	// Game Mode
	TObjectPtr<ACrowdedGameMode> GameMode = Cast<ACrowdedGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GameMode)
		return;

	UWorld* World = GetWorld();
	if (!World)
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
	CamPC->OnRightClickBuild.AddDynamic(this, &UBuildSubsystem::RightClicked);
	
	CamPC->OnLeftRotateBuild.AddDynamic(this, &UBuildSubsystem::TryRotateBuildLeft);
	CamPC->OnRightRotateBuild.AddDynamic(this, &UBuildSubsystem::TryRotateBuildRight);

	// Money component
	const TObjectPtr<ACrowdedGameState> GameState = World->GetGameState<ACrowdedGameState>();
	if (!GameState)
		return;

	MoneyComponent = GameState->GetResourceComponent<EResourceType::Money>();

	// HUD
	GameHUD = Cast<AGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	// Grid actor
	for (TActorIterator<AGridActor> It(GetWorld()); It; ++It)
	{
		GridActor = *It;
		GridActor->ShowGrid(false);
		GridActor->UpdateRoomsVisual();
		break; 
	}

	if (!GridActor)
		UE_LOG(LogTemp, Error, TEXT("BuildSubsystem: couldn't find GridActor in world"));

	BuildableRegistrySubsystem = GetWorld()->GetSubsystem<UBuildableRegistrySubsystem>();
}

TStatId UBuildSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UBuildSubsystem, STATGROUP_Tickables); // Sinon crash quand Tickable true
}

void UBuildSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsSelectingRoom)
	{
		UpdateRoomSelection();
	}
	else
	{
		UpdateGhost();
	}
}

bool UBuildSubsystem::IsTickable() const
{
	return bTickEnabled;
}

#pragma region GameMode / BuildMode / RoomEditMode Changed
void UBuildSubsystem::OnGameModeChanged(const EGameModeState NewMode)
{
	// Activer ou désactiver le tick selon le mode
	bTickEnabled = (NewMode == EGameModeState::Building);

	StopBuilding();

	// UI
	if (NewMode == EGameModeState::Building)
	{
		if (GameHUD)
			GameHUD->ShowBuildWidget(true);
		
		if (GridActor)
		{
			GridActor->SetIsShowingRooms(true);
			GridActor->ShowGrid(true);
			GridActor->ShowPlacedRooms(true);
		}
	}
	else
	{
		if (GameHUD)
			GameHUD->ShowBuildWidget(false);

		if (GridActor)
		{
			GridActor->ShowGrid(false);
			GridActor->UpdateRoomsVisual();
		}

		StopBuilding();
	}
}

void UBuildSubsystem::OnBuildModeSelected(const EBuildModeType BuildMode) const
{
	switch (BuildMode)
	{
		case EBuildModeType::Objects:
			GridActor->SetIsShowingRooms(true);
			GridActor->ShowPlacedRooms(true);
			break;
		case EBuildModeType::Rooms:
			GridActor->SetIsShowingRooms(true);
			GridActor->ShowPlacedRooms(true);
			break;
	}
}

void UBuildSubsystem::ChangeRoomEditMode()
{
	if (CurrentRoomEditMode == ERoomEditMode::Add)
		CurrentRoomEditMode = ERoomEditMode::Remove;
	else
		CurrentRoomEditMode = ERoomEditMode::Add;
}
#pragma endregion

#pragma region Start/Stop Building
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

	// Get materials
	TArray<UMaterialInterface*> Materials;
	if (DefaultBuildable->GetMeshComponent())
	{
		const int32 NumMaterials = DefaultBuildable->GetMeshComponent()->GetNumMaterials();
		for (int32 i = 0; i < NumMaterials; ++i)
		{
			Materials.Add(DefaultBuildable->GetMeshComponent()->GetMaterial(i));
		}
	}

	CurrentGhost->SetMaterials(Materials);

	// Scale
	CurrentGhost->SetActorScale3D(DefaultBuildable->GetMeshComponent()->GetRelativeScale3D());

	MeshOffset = DefaultBuildable->GetMeshComponent()->GetRelativeLocation();

	ResetBuildRotation();

	if (CurrentGhost)
	{
		CurrentGhost->SetActorRotation(CurrentBuildRotation);
	}
}

void UBuildSubsystem::StartRoomBuilding(UBuildRoomData* BuildRoomData)
{
	if (!BuildRoomData)
		return;

	bIsSelectingRoom = true;
	
	CurrentBuildRoomData = BuildRoomData;
	
	if (CurrentGhost)
	{
		CurrentGhost->SetActorHiddenInGame(true);
	}

	ResetBuildRotation();
}

void UBuildSubsystem::StopBuilding()
{
	CurrentBuildData = nullptr;
	CurrentBuildRoomData = nullptr;

	// Stop object selection
	if (CurrentGhost)
		CurrentGhost->SetActorHiddenInGame(true);
	
	SelectedRoomCells.Empty();

	GridActor->SetIsShowingRooms(true);
	ResetBuildRotation();

	GridActor->DeselectSelectedCells();
}
#pragma endregion 

#pragma region Object/Room Creation
void UBuildSubsystem::PlaceObject()
{
	if (!CurrentGhost || !CurrentBuildData || !CurrentBuildData->BuildClass || !GridActor)
		return;

	int SizeX = CurrentBuildData->GridRowsX;
	int SizeY = CurrentBuildData->GridColumnsY;
	
	GetObjectRotatedSize(SizeX, SizeY);

	int StartRow = LastStartRow;
	int StartCol = LastStartCol;
	GridActor->GetCellAtLocation(CurrentGhost->GetActorLocation(), StartRow, StartCol);
	
	StartRow -= SizeX / 2;
	StartCol -= SizeY / 2;

	StartRow = FMath::Clamp(StartRow, 0, GridActor->GetRows() - SizeX);
	StartCol = FMath::Clamp(StartCol, 0, GridActor->GetColumns() - SizeY);
	
	if (CheckIsObjectCuttingRooms(SizeX, SizeY, StartRow, StartCol))
		return;
	
	for (int Row = StartRow; Row < StartRow + SizeX; ++Row)
	{
		for (int Col = StartCol; Col < StartCol + SizeY; ++Col)
		{
			const FGridCell* Cell = GridActor->GetGridCell(Row, Col);
			if (CurrentBuildData->RoomType == EGridRoomType::Any)
			{
				if (!Cell || Cell->bOccupied)
					return;
			}
			else
			{
				if (!Cell || Cell->bOccupied || Cell->RoomType != CurrentBuildData->RoomType)
					return;

				CurrentObjectRoomId = Cell->RoomId;
			}
		}
	}

	FVector SpawnLocation = CurrentGhost->GetActorLocation();
	SpawnLocation -= CurrentBuildRotation.RotateVector(MeshOffset);;

	const TObjectPtr<ABuildableObject> Placed = GetWorld()->SpawnActor<ABuildableObject>(
		CurrentBuildData->BuildClass,
		SpawnLocation,
		CurrentBuildRotation
	);

	Placed->SetBuildData(CurrentBuildData);
	Placed->RoomId = CurrentObjectRoomId;

	if (!Placed)
		return;

	// Default scale
	const TObjectPtr<ABuildableObject> DefaultBuildable = CurrentBuildData->BuildClass->GetDefaultObject<ABuildableObject>();
	Placed->GetMeshComponent()->SetRelativeScale3D(DefaultBuildable->GetMeshComponent()->GetRelativeScale3D());
	Placed->OccupiedCells.Empty();
	
	// Set cells occupied
	for (int Row = StartRow; Row < StartRow + SizeX; ++Row)
	{
		for (int Col = StartCol; Col < StartCol + SizeY; ++Col)
		{
			FGridCell* Cell = GridActor->GetGridCell(Row, Col);
			if (Cell)
			{
				Cell->bOccupied = true;
				Placed->OccupiedCells.Add(FIntPoint(Row, Col));
			}
		}
	}

	if (MoneyComponent)
		MoneyComponent->RemoveResource(CurrentBuildData->MoneyCost);
}

bool UBuildSubsystem::CheckIsObjectCuttingRooms(const int SizeX, const int SizeY, const int StartRow, const int StartCol) const
{
	if (!CurrentGhost || !CurrentBuildData || !CurrentBuildData->BuildClass || !GridActor)
		return true;

	int LastCheckedRoomId = INT_MAX;

	for (int Row = StartRow; Row < StartRow + SizeX; ++Row)
	{
		for (int Col = StartCol; Col < StartCol + SizeY; ++Col)
		{
			const FGridCell* Cell = GridActor->GetGridCell(Row, Col);
			if (LastCheckedRoomId == INT_MAX)
			{
				LastCheckedRoomId = Cell->RoomId;
			}
			else
			{
				if (Cell->RoomId != LastCheckedRoomId)
					return true;

				LastCheckedRoomId = Cell->RoomId;
			}
		}
	}
	
	return false;
}

void UBuildSubsystem::PlaceRoom()
{
	if (!CurrentBuildRoomData || !GridActor)
		return;

	int SizeX = BuildRoomBrushSize;
	int SizeY = BuildRoomBrushSize;

	GetRoomRotatedSize(SizeX, SizeY);

	// Get valid cells
	TArray<FGridCell*> ValidCells;

	for (FGridCell* Cell : SelectedRoomCells)
	{
		if (!Cell) continue;

		if (CurrentRoomEditMode == ERoomEditMode::Add)
		{
			// ADD 
			if (Cell->RoomType == EGridRoomType::None)
			{
				ValidCells.Add(Cell);
			}
		}
		else
		{
			// REMOVE
			if (Cell->RoomType == CurrentBuildRoomData->RoomType)
			{
				ValidCells.Add(Cell);
			}
		}
	}

	if (ValidCells.Num() == 0)
		return;

	if (CurrentRoomEditMode == ERoomEditMode::Add)
	{
		GridActor->AddCellsToRooms(CurrentBuildRoomData, ValidCells);
	}
	else
	{
		GridActor->RemoveCellsFromRooms(ValidCells);
	}

	GridActor->DeselectSelectedCells();
	SelectedRoomCells.Empty();

	GridActor->RebuildWalls();
}
#pragma endregion

#pragma region Object/Room Destroy
void UBuildSubsystem::RemoveObject(const ABuildableObject* Object) const
{
	if (!Object || !GridActor)
		return;
	
	// Set cells unoccupied
	for (const FIntPoint& CellPos : Object->OccupiedCells)
	{
		FGridCell* Cell = GridActor->GetGridCell(CellPos.X, CellPos.Y);
		if (Cell)
		{
			Cell->bOccupied = false;
		}
	}
}

void UBuildSubsystem::DestroyRoom(const int RoomId) const
{
	if (!GridActor)
		return;
	
	if (GridActor->DestroyRoom(RoomId))
	{
		OnRoomDestroyed.Broadcast(RoomId);
	}
}

void UBuildSubsystem::GetObjectsToBeDestroyed(TArray<ABuildableObject*>& OutObjects) const
{
	if (!GridActor) return;

	TSet<FIntPoint> CellsToRemove;

	for (const FGridCell* Cell : SelectedRoomCells)
	{
		if (Cell)
		{
			CellsToRemove.Add(FIntPoint(Cell->Row, Cell->Column));
		}
	}

	for (TWeakObjectPtr<ABuildableObject> Object : BuildableRegistrySubsystem->BuildableObjects)
	{
		if (!Object.IsValid())
			continue;

		for (const FIntPoint& Cell : Object->OccupiedCells)
		{
			if (CellsToRemove.Contains(Cell))
			{
				OutObjects.Add(Object.Get());
				break;
				
			}
		}
	}
}
#pragma endregion

#pragma region Object/Room Rotation
void UBuildSubsystem::TryRotateBuildLeft()
{
	RotationIndex = (RotationIndex + 1) % 4;
	UpdateRotation();
}

void UBuildSubsystem::TryRotateBuildRight()
{
	RotationIndex = (RotationIndex + 3) % 4; 
	UpdateRotation();
}

void UBuildSubsystem::ResetBuildRotation()
{
	RotationIndex = 0;
	CurrentBuildRotation = FRotator(0, 0, 0);
}

void UBuildSubsystem::UpdateRotation()
{
	CurrentBuildRotation = FRotator(0.f, RotationIndex * 90.f, 0.f);
	
	if(CurrentGhost)
		CurrentGhost->SetActorRotation(CurrentBuildRotation);
}

void UBuildSubsystem::GetObjectRotatedSize(int& OutX, int& OutY) const
{
	OutX = CurrentBuildData->GridRowsX;
	OutY = CurrentBuildData->GridColumnsY;

	if (RotationIndex % 2 == 1) // 90 ou 270
	{
		Swap(OutX, OutY);
	}
}

void UBuildSubsystem::GetRoomRotatedSize(int& OutX, int& OutY) const
{
	OutX = BuildRoomBrushSize;
	OutY = BuildRoomBrushSize;

	if (RotationIndex % 2 == 1) // 90 ou 270
	{
		Swap(OutX, OutY);
	}
}
#pragma endregion

#pragma region Get/Set Room Values
TMap<int, FGridRoom>& UBuildSubsystem::GetRooms() const
{
	return GridActor->GetRooms();
}

FGridRoom* UBuildSubsystem::GetRoom(const int RoomId) const
{
	if (!GridActor)
		return 0;
	
	return GridActor->GetRoom(RoomId);
}

float UBuildSubsystem::GetRoomDestroyCost(const int RoomId) const
{
	if (!GridActor)
		return 0;
	
	return GridActor->GetRoomDestroyCost(RoomId);
}

int UBuildSubsystem::GetRoomCellsCount(const int RoomId) const
{
	if (!GridActor)
		return 0;
	
	return GridActor->GetRoom(RoomId)->Cells.Num();
}

void UBuildSubsystem::SetBuildRoomData(const TArray<UBuildRoomData*>& NewBuildRoomData)
{
	BuildDataRooms = NewBuildRoomData;
	
	for (const TObjectPtr<UBuildRoomData> Room : BuildDataRooms)
	{
		UnlockedRooms.Add(Room->RoomType, Room->bIsUnlockedAtStart);
	}
}
#pragma endregion

#pragma region Unlock
void UBuildSubsystem::UnlockRoom(const EGridRoomType RoomType)
{
	UnlockedRooms[RoomType] = true;
}

bool UBuildSubsystem::IsRoomUnlocked(const EGridRoomType RoomType) const 
{
	return UnlockedRooms[RoomType];
}
#pragma endregion

#pragma region Ghost
void UBuildSubsystem::UpdateGhost()
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
	
	GetObjectRotatedSize(SizeX, SizeY);

	int StartRow = HitRow - (SizeX - 1) / 2;
	int StartCol = HitCol - (SizeY - 1) / 2;

	StartRow = FMath::Clamp(StartRow, 0, GridActor->GetRows() - SizeX);
	StartCol = FMath::Clamp(StartCol, 0, GridActor->GetColumns() - SizeY);

	LastStartRow = StartRow;
	LastStartCol = StartCol;
	
	GridActor->DeselectSelectedCells();
	
	if (CheckIsObjectCuttingRooms(SizeX, SizeY, StartRow, StartCol))
	{
		for (int Row = StartRow; Row < StartRow + SizeX; ++Row)
		{
			for (int Col = StartCol; Col < StartCol + SizeY; ++Col)
			{
				FGridCell* Cell = GridActor->GetGridCell(Row, Col);
				if (Cell)
					GridActor->SelectObjectCell(Row, Col, false);
			}
		}
	}
	else
	{
		for (int Row = StartRow; Row < StartRow + SizeX; ++Row)
		{
			for (int Col = StartCol; Col < StartCol + SizeY; ++Col)
			{
				FGridCell* Cell = GridActor->GetGridCell(Row, Col);
				if (Cell)
					GridActor->SelectObjectCell(Row, Col, CurrentBuildData->RoomType);
			}
		}
	}

	FVector2D TopLeft;
	GridActor->GetGridLocation(false, StartRow, StartCol, TopLeft);
	
	FVector SnappedLocation(
		TopLeft.X + (SizeX * GridActor->GetCellSize()) / 2.0f,
		TopLeft.Y + (SizeY * GridActor->GetCellSize()) / 2.0f,
		GridActor->GetActorLocation().Z
	);

	if (MeshOffset != FVector::ZeroVector)
	{
		SnappedLocation += CurrentBuildRotation.RotateVector(MeshOffset);
	}

	CurrentGhost->SetActorLocation(SnappedLocation);
}
#pragma endregion

#pragma region Click & Selection
void UBuildSubsystem::LeftClicked()
{
	if (bIsSelectingRoom)
	{
		PlaceRoom();
	}
	else
	{
		PlaceObject();
	}
}

void UBuildSubsystem::RightClicked()
{
	StopBuilding();

	OnDeselected.Broadcast();
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

void UBuildSubsystem::UpdateRoomSelection()
{
	if(!GridActor)
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

	if (!CurrentBuildRoomData)
		return;
	
	int SizeX = BuildRoomBrushSize;
	int SizeY = BuildRoomBrushSize;

	GetRoomRotatedSize(SizeX, SizeY);

	int StartRow = HitRow - (SizeX - 1) / 2;
	int StartCol = HitCol - (SizeY - 1) / 2;

	StartRow = FMath::Clamp(StartRow, 0, GridActor->GetRows() - SizeX);
	StartCol = FMath::Clamp(StartCol, 0, GridActor->GetColumns() - SizeY);

	SelectedRoomCells.Empty(); // todo: voir pour pas recreer liste a chaque tick...
	GridActor->DeselectSelectedCells();
	for (int Row = StartRow; Row < StartRow + SizeX; ++Row)
	{
		for (int Col = StartCol; Col < StartCol + SizeY; ++Col)
		{
			FGridCell* Cell = GridActor->GetGridCell(Row, Col);
			if (Cell)
			{
				GridActor->SelectRoomCell(Row, Col, CurrentRoomEditMode, CurrentBuildRoomData->RoomType);
				SelectedRoomCells.Add(Cell);
			}
		}
	}

	// Reset disabled material for all objects
	for (TWeakObjectPtr<ABuildableObject> Object : BuildableRegistrySubsystem->BuildableObjects)
	{
		if (Object.IsValid())
		{
			Object->SetWillBeRemoved(false);
		}
	}

	// Set disabled material for object if will be removed by room
	if (CurrentRoomEditMode == ERoomEditMode::Remove)
	{
		TArray<ABuildableObject*> ObjectsToDestroy;
		GetObjectsToBeDestroyed(ObjectsToDestroy);

		for (ABuildableObject* ObjectDestroyedByRoom : ObjectsToDestroy)
		{
			ObjectDestroyedByRoom->SetWillBeRemoved(true);
		}
	}
}

void UBuildSubsystem::OnRoomActiveStateChanged(int RoomId)
{
	if (!GridActor)
		return;
	
	GridActor->UpdateRoomsVisual();
}
#pragma endregion
