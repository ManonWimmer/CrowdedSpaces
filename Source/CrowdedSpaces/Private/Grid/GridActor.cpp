#include "Grid/GridActor.h"

#include "Build/BuildableRegistrySubsystem.h"
#include "Build/BuildData.h"
#include "Grid/GridRoom.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CrowdedPlayerController.h"
#include "Player/PlayerHelpers.h"


AGridActor::AGridActor()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

	LinesProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>("LinesProceduralMesh");
	LinesProceduralMesh->SetupAttachment(RootComponent);

	WallISM = CreateDefaultSubobject<UInstancedStaticMeshComponent>("WallISM");
	WallISM->SetupAttachment(RootComponent);
	WallISM->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WallISM->SetCollisionObjectType(ECC_WorldStatic);
	WallISM->SetCollisionResponseToAllChannels(ECR_Block);
	WallISM->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	WallISM->SetCanEverAffectNavigation(true);

	PrimaryActorTick.bCanEverTick = false;
}

void AGridActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	#pragma region Create Lines
	LinesProceduralMesh->ClearAllMeshSections();
	
	TArray<FVector> LineVertices;
	TArray<int> LineTriangles;

	// Horizontal lines
	for (int i = 0; i <= Rows; i++)
	{
		float LineStart = i * CellSize;
		float LineEnd = LineWidth();

		float Y = i * CellSize;
		DrawLine(FVector(0, Y, 0), FVector(Columns * CellSize, Y, 0), LineThickness, LineVertices, LineTriangles);
	}

	// Vertical lines
	for (int i = 0; i <= Columns; i++)
	{
		float LineStart = i * CellSize;
		float LineEnd = LineHeight();

		float X = i * CellSize;
		DrawLine(FVector(X, 0, 0), FVector(X, Rows * CellSize, 0), LineThickness, LineVertices, LineTriangles);
	}

	// Create line mesh & material
	TArray<FVector> LinesNormals;         
	TArray<FVector2D> LinesUV0;
	const TArray<FColor> LinesVertexColors;
	const TArray<FProcMeshTangent> LinesTangents;

	for (int i = 0; i < LineVertices.Num(); i++)
	{
		LinesNormals.Add(FVector::UpVector); 
		LinesUV0.Add(FVector2D(0,0)); 
	}
	
	LinesProceduralMesh->CreateMeshSection(
		0,           
		LineVertices,
		LineTriangles,
		LinesNormals,
		LinesUV0,
		LinesVertexColors,
		LinesTangents,
		false         
	);

	LinesMaterialInstance = CreateMaterialInstance(LineColor, LineOpacity);
	LinesProceduralMesh->SetMaterial(0, LinesMaterialInstance);
	#pragma endregion
}

void AGridActor::BeginPlay()
{
	Super::BeginPlay();

	#pragma region Create Grid Cells
	Cells.Reserve(Rows * Columns);
	
	for (int Row = 0; Row < Rows; Row++)
	{
		for (int Column = 0; Column < Columns; Column++)
		{
			TObjectPtr<UProceduralMeshComponent> NewCellProceduralMesh = NewObject<UProceduralMeshComponent>(this);
			NewCellProceduralMesh->RegisterComponent();
			NewCellProceduralMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

			FVector NewCellLocation(Row * CellSize,Column * CellSize,0);
			NewCellProceduralMesh->SetRelativeLocation(NewCellLocation);

			NewCellProceduralMesh->SetVisibility(false);
			
			TArray<FVector> NewCellVertices;
			TArray<int> NewCellTriangles;
			float HalfCell = CellSize / 2 - LineThickness / 2;
			
			DrawLine(FVector(0, HalfCell, 0), FVector(CellSize, HalfCell, 0), CellSize, NewCellVertices, NewCellTriangles);

			TArray<FVector> NewCellNormals;         
			TArray<FVector2D> NewCellUV0;           
			TArray<FColor> NewCellVertexColors;     
			TArray<FProcMeshTangent> NewCellTangents;

			for (int x = 0; x < NewCellVertices.Num(); x++)
			{
				NewCellNormals.Add(FVector::UpVector); 
				NewCellUV0.Add(FVector2D(0,0)); 
			}
	
			NewCellProceduralMesh->CreateMeshSection(
				0,           
				NewCellVertices,
				NewCellTriangles,
				NewCellNormals,
				NewCellUV0,
				NewCellVertexColors,
				NewCellTangents,
				false         
			);

			const FLinearColor RandomColor = FLinearColor::MakeRandomColor(); // For tests

			TObjectPtr<UMaterialInstanceDynamic> NewCellMaterialInstance = CreateMaterialInstance(RandomColor, CellOpacity);
			NewCellProceduralMesh->SetMaterial(0, NewCellMaterialInstance);
			
			FIntPoint Key(Row, Column);

			Cells.Emplace(
				Key,
				FGridCell(Row, Column, false, NewCellProceduralMesh, NewCellMaterialInstance));
		}
	}
	#pragma endregion

	const UWorld* World = GetWorld();
	if (!World)
		return;
	
	BuildSubsystem = World->GetSubsystem<UBuildSubsystem>();

	BuildableRegistrySubsystem = GetWorld()->GetSubsystem<UBuildableRegistrySubsystem>();
}

#pragma region Cells / Grid
bool AGridActor::CheckIsValidCell(const int Row, const int Column) const
{
	return (Row >= 0 && Row < Rows && Column >= 0 && Column < Columns);
}

bool AGridActor::GetCellAtLocation(const FVector Location, int& OutRow, int& OutColumn) const
{
	float LocalX = Location.X - GetActorLocation().X;
	float LocalY = Location.Y - GetActorLocation().Y;
	
	OutColumn = FMath::FloorToInt(LocalY / CellSize); 
	OutRow = FMath::FloorToInt(LocalX / CellSize);
	
	return CheckIsValidCell(OutRow, OutColumn);
}

bool AGridActor::GetGridLocation(const bool bIsCenter, const int Row, const int Column, FVector2D& OutGridLocation) const
{
	if (!CheckIsValidCell(Row, Column))
		return false;

	OutGridLocation.X = Row * CellSize + GetActorLocation().X;
	OutGridLocation.Y = Column * CellSize + GetActorLocation().Y;
	
	if (bIsCenter)
	{
		OutGridLocation.X += CellSize / 2;
		OutGridLocation.Y += CellSize / 2;
	}
	
	return true;
}

FGridCell* AGridActor::GetGridCell(int Row, int Column)
{
	return Cells.Find(FIntPoint(Row, Column));
}
#pragma endregion

#pragma region Show / Select
void AGridActor::SelectObjectCell(const int Row, const int Column, const EGridRoomType RoomType)
{
	FGridCell* NewSelectedCell = GetGridCell(Row, Column);
	if (!NewSelectedCell)
		return;
	
	NewSelectedCell->CellProceduralMesh->SetVisibility(true);
	NewSelectedCell->CellProceduralMesh->SetScalarParameterValueOnMaterials("Power", CellSelectionColorPower);

	if (RoomType == EGridRoomType::Any)
	{
		if (NewSelectedCell->bOccupied)
			NewSelectedCell->DynamicMaterial->SetVectorParameterValue(TEXT("Color"), FColor::Red);
		else
			NewSelectedCell->DynamicMaterial->SetVectorParameterValue(TEXT("Color"), FColor::Green);
	}
	else
	{
		if (NewSelectedCell->bOccupied || NewSelectedCell->RoomType != RoomType)
			NewSelectedCell->DynamicMaterial->SetVectorParameterValue(TEXT("Color"), FColor::Red);
		else
			NewSelectedCell->DynamicMaterial->SetVectorParameterValue(TEXT("Color"), FColor::Green);
	}
	
	SelectedCells.Add(NewSelectedCell);
}

void AGridActor::SelectObjectCell(const int Row, const int Column, const bool bSelect)
{
	FGridCell* NewSelectedCell = GetGridCell(Row, Column);
	if (!NewSelectedCell)
		return;
	
	NewSelectedCell->CellProceduralMesh->SetVisibility(true);
	NewSelectedCell->CellProceduralMesh->SetScalarParameterValueOnMaterials("Power", CellSelectionColorPower);

	if (bSelect)
	{
		NewSelectedCell->DynamicMaterial->SetVectorParameterValue(TEXT("Color"), FColor::Green);
	}
	else
	{
		NewSelectedCell->DynamicMaterial->SetVectorParameterValue(TEXT("Color"), FColor::Red);
	}
	
	SelectedCells.Add(NewSelectedCell);
}

void AGridActor::SelectRoomCell(const int Row, const int Column, const ERoomEditMode EditMode, const EGridRoomType TargetRoomType)
{
	FGridCell* NewSelectedCell = GetGridCell(Row, Column);
	if (!NewSelectedCell)
		return;
	
	NewSelectedCell->CellProceduralMesh->SetVisibility(true);

	if (EditMode == ERoomEditMode::Add)
	{
		// ADD
		if (NewSelectedCell->RoomType != EGridRoomType::None)
			NewSelectedCell->DynamicMaterial->SetVectorParameterValue(TEXT("Color"), FColor::Red);
		else
			NewSelectedCell->DynamicMaterial->SetVectorParameterValue(TEXT("Color"), FColor::Green);
	}
	else
	{
		// REMOVE
		if (NewSelectedCell->RoomType == TargetRoomType)
			NewSelectedCell->DynamicMaterial->SetVectorParameterValue(TEXT("Color"), FColor::Green);
		else
			NewSelectedCell->DynamicMaterial->SetVectorParameterValue(TEXT("Color"), FColor::Red);
	}

	SelectedCells.Add(NewSelectedCell);
}

void AGridActor::ShowPlacedRooms(bool bShow)
{
	for (TTuple<int, FGridRoom> Room : Rooms)
	{
		for (FGridCell* Cell : Room.Value.Cells)
		{
			if (bShow)
			{
				Cell->CellProceduralMesh->SetVisibility(true);
				Cell->DynamicMaterial->SetVectorParameterValue(TEXT("Color"), Room.Value.GridColor);
				Cell->CellProceduralMesh->SetScalarParameterValueOnMaterials("Power", CellSelectionColorPower);
			}
			else
			{
				Cell->CellProceduralMesh->SetVisibility(false);
			}
		}
	}
}

void AGridActor::ShowGrid(bool bShow)
{
	//LinesProceduralMesh->SetVisibility(bShow);

	for (auto& Pair : Cells)
	{
		FGridCell* Cell = &Pair.Value;
		if (!Cell)
			continue;

		Cell->CellProceduralMesh->SetVisibility(false);
		Cell->CellProceduralMesh->SetScalarParameterValueOnMaterials("Power", CellSelectionColorPower);
	}
}

void AGridActor::UpdateRoomsVisual()
{
	// reset tout
	for (int Row = 0; Row < Rows; ++Row)
	{
		for (int Col = 0; Col < Columns; ++Col)
		{
			FGridCell* Cell = GetGridCell(Row, Col);
			if (!Cell || !Cell->CellProceduralMesh) continue;
			
			Cell->CellProceduralMesh->SetVisibility(true);
			Cell->DynamicMaterial->SetVectorParameterValue(TEXT("Color"), NormalCellColor);
			Cell->CellProceduralMesh->SetScalarParameterValueOnMaterials("Power", NormalCellColorPower);
		}
	}

	// apply rooms actives
	for (auto& Pair : Rooms)
	{
		FGridRoom& Room = Pair.Value;

		if (Room.bIsActivated)
		{
			for (const FGridCell* Cell : Room.Cells)
			{
				if (!Cell || !Cell->CellProceduralMesh) continue;

				Cell->CellProceduralMesh->SetVisibility(true);
				Cell->DynamicMaterial->SetVectorParameterValue(TEXT("Color"), ActivatedCellColor);
				Cell->CellProceduralMesh->SetScalarParameterValueOnMaterials("Power", ActivatedCellColorPower);
			}
		}
		else
		{
			for (const FGridCell* Cell : Room.Cells)
			{
				if (!Cell || !Cell->CellProceduralMesh) continue;

				Cell->CellProceduralMesh->SetVisibility(true);
				Cell->DynamicMaterial->SetVectorParameterValue(TEXT("Color"), DeactivatedCellColor);
				Cell->CellProceduralMesh->SetScalarParameterValueOnMaterials("Power", DeactivatedCellColorPower);
			}
		}
	}
}

void AGridActor::DeselectSelectedCells()
{
	for (const FGridCell* Cell : SelectedCells)
	{
		if (Cell)
		{
			FLinearColor OutGridColor = FLinearColor::White;
			if (bIsShowingRooms && CheckIfCellInPlacedRoom(Cell, OutGridColor))
			{
				Cell->CellProceduralMesh->SetVisibility(true);
				Cell->DynamicMaterial->SetVectorParameterValue(TEXT("Color"), OutGridColor);
				Cell->CellProceduralMesh->SetScalarParameterValueOnMaterials("Power", CellSelectionColorPower);
			}
			else
			{
				Cell->CellProceduralMesh->SetVisibility(false); 
			}
		}
	}

	SelectedCells.Empty();
}
#pragma endregion

#pragma region Rooms
FGridRoom* AGridActor::GetRoomOfSameType(EGridRoomType RoomType, int Row, int Col)
{
	FGridCell* Cell = GetGridCell(Row, Col);
	if (!Cell)
		return nullptr;
	
	FGridRoom* Room = GetRoomAtCell(Cell);
	if (Room && Room->RoomType == RoomType)
		return Room;

	return nullptr;
}

FGridRoom* AGridActor::GetRoom(const int RoomId)
{
	return Rooms.Find(RoomId);
}

bool AGridActor::CheckIfCellInPlacedRoom(const FGridCell* Cell, FLinearColor& OutGridColor)
{
	if (Cell->RoomType != EGridRoomType::None)
	{
		const FGridRoom* Room = GetRoom(Cell->RoomId);
		if (Room)
		{
			OutGridColor = Room->GridColor;
			return true;
		}
	}

	return false;
}

bool AGridActor::DestroyRoom(const int RoomId)
{
	FGridRoom* RoomToDestroy = GetRoom(RoomId);
	if (!RoomToDestroy)
		return false;
		
	float RoomDestroyMoney = GetRoomDestroyCost(RoomId);
	
	for (const FGridCell* RoomCell : RoomToDestroy->Cells)
	{
		FGridCell* GridCell = Cells.Find(FIntPoint(RoomCell->Row, RoomCell->Column));
		GridCell->CellType = EGridCellType::None;
		GridCell->RoomId = BuildSubsystem->InvalidRoomId;
		GridCell->RoomType = EGridRoomType::None;
		RoomCell->CellProceduralMesh->SetVisibility(false);
	}

	Rooms.Remove(RoomId);
	
	DeselectSelectedCells(); // Bizarre que ça deselect pas les rooms tout seul, c'est le set visibility au dessus qui fait (plus tard maybe bugs)
	
	RebuildWalls();
	
	for (TWeakObjectPtr<AUsableObject> Object : BuildableRegistrySubsystem->BuildableObjects)
	{
		if (!Object.IsValid())
			continue;

		if (Object->RoomId != RoomId)
			continue;
		
		Object->DestroyObject();
	}
	
	UResourceComponent* PlayerMoneyComponent = PlayerHelpers::GetPlayerResourceComponent(*GetWorld(), EResourceType::Money);
	if (PlayerMoneyComponent)
		PlayerMoneyComponent->AddResource(RoomDestroyMoney);
	
	// todo: pareil que plus haut
	ACrowdedPlayerController* CrowdedPlayerController = Cast<ACrowdedPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	AGameHUD * GameHUD = Cast<AGameHUD>(CrowdedPlayerController->GetHUD());
	GameHUD->HideCurrentSelectionWidget();

	return true;
}

float AGridActor::GetRoomDestroyCost(int RoomId)
{
	const FGridRoom* Room = GetRoom(RoomId);
	if (!Room)
		return 0.f;
	
	float RoomTotalDestroyCost = Room->DestroyMoneyPerCell * Room->Cells.Num();

	for (TWeakObjectPtr<AUsableObject> Object : BuildableRegistrySubsystem->BuildableObjects)
	{
		if (!Object.IsValid())
			continue;

		if (Object->RoomId != RoomId)
			continue;

		GEngine->AddOnScreenDebugMessage(-1,5,FColor::Green,"Objet in room");
		RoomTotalDestroyCost += Object->GetBuildData()->DestroyMoney;
	}
	
	return RoomTotalDestroyCost;
}

FGridRoom* AGridActor::GetRoomAtCell(const FGridCell* Cell)
{
	if (!Cell || Cell->RoomType == EGridRoomType::None)
		return nullptr;
	
	return GetRoom(Cell->RoomId);
}

bool AGridActor::GetRoomAtWorldLocation(const FVector& WorldLoc, FGridRoom*& OutRoom)
{
	int Row, Col;

	if (!GetCellAtLocation(WorldLoc, Row, Col))
		return false;

	FGridCell* Cell = GetGridCell(Row, Col);
	if (!Cell || Cell->RoomType == EGridRoomType::None)
		return false;

	OutRoom = GetRoom(Cell->RoomId);

	return OutRoom != nullptr;
}

const UBuildRoomData* AGridActor::GetRoomDataFromType(const EGridRoomType RoomType) const
{
	for (const UBuildRoomData* Data : BuildSubsystem->GetBuildDataRooms())
	{
		if (Data && Data->RoomType == RoomType)
			return Data;
	}

	return nullptr;
}

void AGridActor::RecomputeAllRooms()
{
	// Clean all rooms
	Rooms.Empty();

	TSet<FGridCell*> Visited;

	for (auto& Pair : Cells)
	{
		FGridCell* StartCell = &Pair.Value;

		if (!StartCell)
			continue;

		if (StartCell->RoomType == EGridRoomType::None)
			continue;

		if (Visited.Contains(StartCell))
			continue;

		// Create room
		FGridRoom NewRoom;
		NewRoom.RoomId = NextRoomId++;
		NewRoom.RoomType = StartCell->RoomType;
		
		const UBuildRoomData* RoomData = GetRoomDataFromType(StartCell->RoomType);
		if (RoomData)
		{
			NewRoom.GridColor = RoomData->GridColor;
			NewRoom.LoseElectricityPerHourPerCell = RoomData->LoseElectricityPerHour;
			NewRoom.DestroyMoneyPerCell = RoomData->DestroyMoneyPerCell;
		}

		// Check around neighbors cells with flood fill (DFS with stack)
		TArray<FGridCell*> Stack;
		Stack.Add(StartCell);

		while (Stack.Num() > 0)
		{
			FGridCell* Cell = Stack.Pop();

			if (!Cell)
				continue;

			// Already visited
			if (Visited.Contains(Cell))
				continue;

			// Wrong room type
			if (Cell->RoomType != NewRoom.RoomType)
				continue;

			Visited.Add(Cell);

			// Add to new room
			Cell->RoomId = NewRoom.RoomId;
			NewRoom.Cells.Add(Cell);

			// Check cell neighbors
			const TArray<FIntPoint> Neighbors = {
				{Cell->Row + 1, Cell->Column},
				{Cell->Row - 1, Cell->Column},
				{Cell->Row, Cell->Column + 1},
				{Cell->Row, Cell->Column - 1}
			};

			for (const FIntPoint& N : Neighbors)
			{
				FGridCell* Neighbor = GetGridCell(N.X, N.Y);

				if (!Neighbor)
					continue;

				if (Visited.Contains(Neighbor))
					continue;

				// Same type : add neighbor to explore stack
				if (Neighbor->RoomType == NewRoom.RoomType)
				{
					Stack.Add(Neighbor);
				}
			}
		}

		// Create final room
		Rooms.Add(NewRoom.RoomId, NewRoom);
	}

	// Update objects new ids
	for (const TWeakObjectPtr<AUsableObject> Object : BuildableRegistrySubsystem->BuildableObjects)
	{
		FGridRoom* Room;
		if (GetRoomAtWorldLocation(Object->GetActorLocation(), Room))
		{
			Object->RoomId = Room->RoomId;
		}
		else
		{
			Object->DestroyObject();
		}
	}

	ShowPlacedRooms(true);
	BuildSubsystem->OnRoomsRecomputed.Broadcast();
	WallISM->MarkRenderStateDirty();
	WallISM->UpdateNavigationBounds();
}

void AGridActor::AddCellsToRooms(TObjectPtr<UBuildRoomData> BuildData, TArray<FGridCell*> CellsToAdd)
{
	for (FGridCell* Cell : CellsToAdd)
	{
		if (!Cell) continue;

		Cell->RoomType = BuildData->RoomType;
	}

	RecomputeAllRooms();
}

void AGridActor::RemoveCellsFromRooms(TArray<FGridCell*> CellsToRemove)
{
	TSet<FIntPoint> RemovedCells;

	// Remove room from cell
	for (FGridCell* Cell : CellsToRemove)
	{
		if (!Cell)
			continue;
		
		RemovedCells.Add(FIntPoint(Cell->Row, Cell->Column));
		Cell->RoomId = BuildSubsystem->InvalidRoomId;
		Cell->RoomType = EGridRoomType::None;
		Cell->bOccupied = false;
	}

	// Remove object on cells to remove
	TArray<TWeakObjectPtr<AUsableObject>> ObjectsCopy = BuildableRegistrySubsystem->BuildableObjects;

	for (TWeakObjectPtr<AUsableObject> Object : ObjectsCopy)
	{
		if (!Object.IsValid())
			continue;

		if (Object->IsOverlappingCells(RemovedCells))
		{
			Object->DestroyObject();
		}
	}
	
	RecomputeAllRooms();
}

bool AGridActor::GetRandomCellWorldPosition(FVector& OutWorldPos, int& Row, int& Column, const bool bCenter)
{
	if (Cells.Num() == 0)
		return false;

	TArray<FIntPoint> Keys;
	Cells.GetKeys(Keys);

	const FIntPoint& RandomKey = Keys[FMath::RandRange(0, Keys.Num() - 1)];
	const FGridCell* Cell = Cells.Find(RandomKey);

	if (!Cell)
		return false;

	Row = Cell->Row;
	Column = Cell->Column;

	FVector2D GridPos;
	if (!GetGridLocation(bCenter, Cell->Row, Cell->Column, GridPos))
		return false;

	OutWorldPos = FVector(GridPos.X, GridPos.Y, GetActorLocation().Z);
	return true;
}
#pragma endregion

#pragma region Walls
void AGridActor::RebuildWalls()
{
	WallISM->ClearInstances();
	CreatedWallsPositions.Empty();
	TArray<FIntPoint> BorderCells;
	
	CollectBorderCells(BorderCells);
	
	TArray<FWallSegment> Segments;
	BuildWallSegments(BorderCells, Segments);
	
	for (FWallSegment& Seg : Segments)
	{
		SpawnSegment(Seg);
	}
}

void AGridActor::BuildWallSegments(const TArray<FIntPoint>& CellsToCheck, TArray<FWallSegment>& OutSegments)
{
	TMap<int, TArray<int>> CurrentRows;
	TMap<int, TArray<int>> CurrentCols;

	// split horizontal / vertical
	for (const FIntPoint& C : CellsToCheck)
	{
		CurrentRows.FindOrAdd(C.X).Add(C.Y);
		CurrentCols.FindOrAdd(C.Y).Add(C.X);
	}
	
	// horizontal segments
	for (auto& Pair : CurrentRows)
	{
		Pair.Value.Sort(); ExtractSegments(Pair.Key, Pair.Value, true, OutSegments);
	}
	
	// vertical segments
	for (auto& Pair : CurrentCols)
	{
		Pair.Value.Sort();
		ExtractSegments(Pair.Key, Pair.Value, false, OutSegments);
	}
}

void AGridActor::ExtractSegments(int Fixed, const TArray<int>& Values, bool bHorizontal, TArray<FWallSegment>& OutSegments)
{
	int Start = 0;
	while (Start < Values.Num())
	{
		int End = Start;
		while (End + 1 < Values.Num() && Values[End + 1] == Values[End] + 1)
		{
			End++;
		}
		
		FWallSegment Seg; Seg.bHorizontal = bHorizontal;
		for (int i = Start; i <= End; i++)
		{
			FIntPoint P = bHorizontal ? FIntPoint(Fixed, Values[i]) : FIntPoint(Values[i], Fixed); Seg.Cells.Add(P);
		}

		OutSegments.Add(Seg); Start = End + 1;
	}
}

void AGridActor::SpawnWallWithDoorLogic(FGridCell* Cell, bool bHorizontal, float Half)
{
	if (!Cell)
		return;
	
	auto TrySpawn = [&](int NR, int NC, EGridWallDirection Dir)
	{
		const FGridCell* Neighbor = GetGridCell(NR, NC);
		
		if (Neighbor && Neighbor->RoomType == Cell->RoomType)
			return; FVector2D CellPos;
		
		if (!GetGridLocation(true, Cell->Row, Cell->Column, CellPos))
			return;

		FVector SpawnLoc( CellPos.X - GetActorLocation().X, CellPos.Y - GetActorLocation().Y, 0 );
		FRotator Rot = FRotator::ZeroRotator;
		
		switch (Dir)
		{
			case EGridWallDirection::North: SpawnLoc.X -= Half;
				break;
			case EGridWallDirection::South: SpawnLoc.X += Half;
				break;
			case EGridWallDirection::West: SpawnLoc.Y -= Half; Rot = FRotator(0, 90, 0);
				break;
			case EGridWallDirection::East: SpawnLoc.Y += Half; Rot = FRotator(0, 90, 0);
				break;
		}
		
		if (CreatedWallsPositions.Contains(SpawnLoc))
			return;

		const bool bCreatePassage = DoorCells.Contains(FIntPoint(Cell->Row, Cell->Column));
		if (bCreatePassage)
		{
			constexpr float PassageHeight = 180.f;
			constexpr float WallHeight = 223.f;
			const FVector TopLoc = SpawnLoc + FVector(0, 0, PassageHeight + (WallHeight - PassageHeight) / 2); 
			const FVector TopScale( 1.f, 1.f, (WallHeight - PassageHeight) / WallHeight );
			WallISM->AddInstance(FTransform(Rot, TopLoc, TopScale));
		}
		else
		{
			WallISM->AddInstance(FTransform(Rot, SpawnLoc));
		}

		CreatedWallsPositions.Add(SpawnLoc);
	};

	// 4 directions
	TrySpawn(Cell->Row - 1, Cell->Column, EGridWallDirection::North);
	TrySpawn(Cell->Row + 1, Cell->Column, EGridWallDirection::South);
	TrySpawn(Cell->Row, Cell->Column - 1, EGridWallDirection::West);
	TrySpawn(Cell->Row, Cell->Column + 1, EGridWallDirection::East);
}

void AGridActor::SpawnWallPerCell(FGridCell* Cell, bool bHorizontal, float Half, bool bIsDoor)
{
	if (!Cell) return;

	auto Spawn = [&](int NR, int NC, EGridWallDirection Dir)
	{
		FGridCell* Neighbor = GetGridCell(NR, NC);

		if (Neighbor && Neighbor->RoomId == Cell->RoomId)
			return;

		FVector2D CellPos;
		if (!GetGridLocation(true, Cell->Row, Cell->Column, CellPos))
			return;

		FVector SpawnLoc(CellPos.X - GetActorLocation().X, CellPos.Y - GetActorLocation().Y, 0);

		FRotator Rot = FRotator::ZeroRotator;

		switch (Dir)
		{
			case EGridWallDirection::North: SpawnLoc.X -= Half;
				break;
			case EGridWallDirection::South: SpawnLoc.X += Half;
				break;
			case EGridWallDirection::West:  SpawnLoc.Y -= Half; Rot = FRotator(0, 90, 0);
				break;
			case EGridWallDirection::East:  SpawnLoc.Y += Half; Rot = FRotator(0, 90, 0);
				break;
		}

		if (CreatedWallsPositions.Contains(SpawnLoc))
			return;

		if (bIsDoor)
		{
			constexpr float PassageHeight = 180.f;
			constexpr float WallHeight = 223.f;

			const FVector TopLoc = SpawnLoc + FVector(0, 0, PassageHeight + (WallHeight - PassageHeight) / 2);

			const FVector TopScale(1.f, 1.f, (WallHeight - PassageHeight) / WallHeight);

			WallISM->AddInstance(FTransform(Rot, TopLoc, TopScale));
		}
		else
		{
			WallISM->AddInstance(FTransform(Rot, SpawnLoc));
		}

		CreatedWallsPositions.Add(SpawnLoc);
	};
	
	if (bHorizontal)
	{
		Spawn(Cell->Row - 1, Cell->Column, EGridWallDirection::North);
		Spawn(Cell->Row + 1, Cell->Column, EGridWallDirection::South);
	}
	else
	{
		Spawn(Cell->Row, Cell->Column - 1, EGridWallDirection::West);
		Spawn(Cell->Row, Cell->Column + 1, EGridWallDirection::East);
	}
}

void AGridActor::SpawnSegment(const FWallSegment& Seg)
{
	constexpr float Half = 50.f;

	if (Seg.Cells.Num() == 0)
		return;

	const bool bHasDoor = Seg.Cells.Num() >= MinWallSizeForDoors;
	const int DoorIndex = bHasDoor ? Seg.Cells.Num() / 2 : BuildSubsystem->InvalidRoomId;

	for (int i = 0; i < Seg.Cells.Num(); i++)
	{
		FGridCell* Cell = GetGridCell(Seg.Cells[i].X, Seg.Cells[i].Y);
		if (!Cell) continue;

		const bool bIsDoor = (i == DoorIndex);

		SpawnWallPerCell(Cell, Seg.bHorizontal, Half, bIsDoor);
	}
}

void AGridActor::CollectBorderCells(TArray<FIntPoint>& OutCells)
{
	for (auto& RoomPair : Rooms)
	{
		FGridRoom& Room = RoomPair.Value;
		for (const FGridCell* Cell : Room.Cells)
		{
			if (!Cell)
				continue;
			
			int R = Cell->Row;
			int C = Cell->Column;

			// check 4 neighbors
			if (IsBorder(Cell, R + 1, C) || IsBorder(Cell, R - 1, C) || IsBorder(Cell, R, C + 1) || IsBorder(Cell, R, C - 1))
			{
				OutCells.Add(FIntPoint(R, C));
			}
		}
	}
}

bool AGridActor::IsBorder(const FGridCell* Cell, const int R, const int C)
{
	const FGridCell* N = GetGridCell(R, C);

	return (!N || N->RoomId != Cell->RoomId);
}
#pragma endregion

#pragma region Visuals
void AGridActor::DrawLine(const FVector& Start, const FVector& End, const float Thickness, TArray<FVector>& Vertices, TArray<int>& Triangles)
{
	const float HalfThickness = Thickness / 2;

	FVector Direction = End - Start;
	Direction.Normalize();
	Direction = FVector::CrossProduct(Direction, FVector(0,0,1));

	const int StartIndex = Vertices.Num();
	Vertices.Add(Start + (Direction * HalfThickness));	// Top left corner
	Vertices.Add(End + (Direction * HalfThickness));	// Top right corner
	Vertices.Add(Start - (Direction * HalfThickness));	// Bottom left corner
	Vertices.Add(End - (Direction * HalfThickness));	// Bottom right corner

	Triangles.Add(StartIndex + 0);
	Triangles.Add(StartIndex + 2);
	Triangles.Add(StartIndex + 1);

	Triangles.Add(StartIndex + 2);
	Triangles.Add(StartIndex + 3);
	Triangles.Add(StartIndex + 1);
}

float AGridActor::LineWidth() const
{
	return Rows * CellSize;
}

float AGridActor::LineHeight() const
{
	return Columns * CellSize;
}

TObjectPtr<UMaterialInstanceDynamic> AGridActor::CreateMaterialInstance(const FLinearColor Color, const float Opacity)
{
	if (!BaseMaterial)
	{
		UE_LOG(LogTemp, Warning, TEXT("BaseMaterial is null!"));
		return nullptr;
	}

	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);

	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue(TEXT("Color"), Color);
		DynamicMaterial->SetScalarParameterValue(TEXT("Opacity"), Opacity);
	}

	return DynamicMaterial;
}
#pragma endregion



