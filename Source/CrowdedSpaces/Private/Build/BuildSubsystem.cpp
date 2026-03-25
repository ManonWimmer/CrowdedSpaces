#include "Build/BuildSubsystem.h"

#include "EngineUtils.h"
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
		
		if (GridActor)
		{
			GridActor->SetIsShowingRooms(true);
			GridActor->ShowPlacedRooms(true);
			GridActor->ShowGrid(true);
		}
	}
	else
	{
		if (GameHUD)
			GameHUD->ShowBuildWidget(false);

		if (GridActor)
			GridActor->ShowGrid(false);

		StopBuilding();
	}
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

void UBuildSubsystem::OnBuildModeSelected(EBuildModeType BuildMode)
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

void UBuildSubsystem::PlaceObject() const
{
	if (!CurrentGhost || !CurrentBuildData || !CurrentBuildData->BuildClass || !GridActor)
		return;

	int SizeX = CurrentBuildData->GridRowsX;
	int SizeY = CurrentBuildData->GridColumnsY;
	
	GetObjectRotatedSize(SizeX, SizeY);

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

				CurrentBuildData->RoomId = Cell->RoomId;
			}
		}
	}

	FVector SpawnLocation = CurrentGhost->GetActorLocation();
	SpawnLocation -= MeshOffset;

	const TObjectPtr<ABuildableObject> Placed = GetWorld()->SpawnActor<ABuildableObject>(
		CurrentBuildData->BuildClass,
		SpawnLocation,
		CurrentBuildRotation
	);

	Placed->SetBuildData(CurrentBuildData);

	if (!Placed)
		return;

	// Default scale
	TObjectPtr<ABuildableObject> DefaultBuildable = CurrentBuildData->BuildClass->GetDefaultObject<ABuildableObject>();
	Placed->GetMeshComponent()->SetRelativeScale3D(DefaultBuildable->GetMeshComponent()->GetRelativeScale3D());

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
		MoneyComponent->RemoveResource(CurrentBuildData->MoneyCost);
}

void UBuildSubsystem::RemoveObject(ABuildableObject* Object) const
{
	int SizeX = Object->GetBuildData()->GridRowsX;
	int SizeY = Object->GetBuildData()->GridColumnsY;

	// Rotation
	if (Object->GetActorRotation() == FRotator(0.f, 90.f, 0.f) ||
		Object->GetActorRotation() == FRotator(0.f, 270.f, 0.f)) 
	{
		Swap(SizeX, SizeY);
	}
	
	int StartRow = 0, StartCol = 0;
	GridActor->GetCellAtLocation(Object->GetActorLocation(), StartRow, StartCol);
	
	StartRow -= SizeX / 2;
	StartCol -= SizeY / 2;

	StartRow = FMath::Clamp(StartRow, 0, GridActor->GetRows() - SizeX);
	StartCol = FMath::Clamp(StartCol, 0, GridActor->GetColumns() - SizeY);
	
	// Set cells unoccupied
	for (int Row = StartRow; Row < StartRow + SizeX; ++Row)
	{
		for (int Col = StartCol; Col < StartCol + SizeY; ++Col)
		{
			FGridCell* Cell = GridActor->GetGridCell(Row, Col);
			if (Cell)
				Cell->bOccupied = false;
		}
	}
	
	if (MoneyComponent)
		MoneyComponent->AddResource(Object->GetBuildData()->DestroyMoney);
}

void UBuildSubsystem::PlaceRoom()
{
	if (!CurrentBuildRoomData || !GridActor)
		return;

	int SizeX = CurrentBuildRoomData->GridRowsX;
	int SizeY = CurrentBuildRoomData->GridColumnsY;

	GetRoomRotatedSize(SizeX, SizeY);
	
	// Check can place
	for (const FGridCell* Cell : SelectedRoomCells)
	{
		if (!Cell)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlaceRoom: SelectedRoomCells contient nullptr !"));
			return;
		}

		if (Cell->RoomType != EGridRoomType::None)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlaceRoom: SelectedRoomCells contient une room non set"));
			return;
		}
	}
	
	GridActor->CreateRoom(CurrentBuildRoomData, SelectedRoomCells);

	if (MoneyComponent)
		MoneyComponent->RemoveResource(CurrentBuildRoomData->MoneyCost);

	GridActor->DeselectSelectedCells();
	
	SelectedRoomCells.Empty();
}

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
	OutX = CurrentBuildRoomData->GridRowsX;
	OutY = CurrentBuildRoomData->GridColumnsY;

	if (RotationIndex % 2 == 1) // 90 ou 270
	{
		Swap(OutX, OutY);
	}
}

TMap<int, FGridRoom>& UBuildSubsystem::GetRooms()
{
	return GridActor->GetRooms();
}

void UBuildSubsystem::DestroyRoom(int RoomId)
{
	if (!GridActor)
		return;
	
	GridActor->DestroyRoom(RoomId);
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
	
	GetObjectRotatedSize(SizeX, SizeY);

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
				GridActor->SelectObjectCell(Row, Col, CurrentBuildData->RoomType);
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
		SnappedLocation += MeshOffset;
	}

	CurrentGhost->SetActorLocation(SnappedLocation);

	CurrentGhost->SetActorLocation(SnappedLocation);
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

	int SizeX = CurrentBuildRoomData->GridRowsX;
	int SizeY = CurrentBuildRoomData->GridColumnsY;

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
				GridActor->SelectRoomCell(Row, Col);
				SelectedRoomCells.Add(Cell);
			}
		}
	}
}

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
