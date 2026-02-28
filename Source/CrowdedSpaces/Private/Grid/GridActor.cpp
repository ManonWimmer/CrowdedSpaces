#include "Grid/GridActor.h"

#include "Grid/GridRoom.h"


AGridActor::AGridActor()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	
	LinesProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>("LinesProceduralMesh");
	LinesProceduralMesh->SetupAttachment(RootComponent);

	WallISM = CreateDefaultSubobject<UInstancedStaticMeshComponent>("WallISM");
	WallISM->SetupAttachment(RootComponent);
	WallISM->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WallISM->SetCollisionObjectType(ECC_WorldStatic);
	WallISM->SetCollisionResponseToAllChannels(ECR_Block);
	WallISM->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	
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
				FGridCell(Row, Column, false, EGridCellType::None, EGridRoomType::None,
				NewCellProceduralMesh, NewCellMaterialInstance));
		}
	}
	#pragma endregion 
}

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

void AGridActor::SelectObjectCell(const int Row, const int Column, EGridRoomType RoomType)
{
	FGridCell* NewSelectedCell = GetGridCell(Row, Column);
	if (!NewSelectedCell)
		return;
	
	NewSelectedCell->CellProceduralMesh->SetVisibility(true);

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

void AGridActor::SelectRoomCell(const int Row, const int Column)
{
	FGridCell* NewSelectedCell = GetGridCell(Row, Column);
	if (!NewSelectedCell)
		return;
	
	NewSelectedCell->CellProceduralMesh->SetVisibility(true);

	if (NewSelectedCell->RoomType != EGridRoomType::None)
		NewSelectedCell->DynamicMaterial->SetVectorParameterValue(TEXT("Color"), FColor::Red);
	else
		NewSelectedCell->DynamicMaterial->SetVectorParameterValue(TEXT("Color"), FColor::Green); 
	
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
			}
			else
			{
				Cell->CellProceduralMesh->SetVisibility(false);
			}
		}
	}
}

int AGridActor::CreateRoom(const UBuildRoomData* BuildData, TArray<FGridCell*> CellsToAssign)
{
	FGridRoom NewRoom;
	NewRoom.RoomId = NextRoomId++;
	NewRoom.RoomType = BuildData->RoomType;
	NewRoom.GridColor = BuildData->GridColor;

	for (FGridCell* Cell : CellsToAssign)
	{
		Cell->RoomId = NewRoom.RoomId;
		Cell->RoomType = NewRoom.RoomType;

		NewRoom.Cells.Add(Cell);
	}

	Rooms.Add(NewRoom.RoomId, NewRoom);

	ShowPlacedRooms(true);
	
	RebuildWalls();
	
	return NewRoom.RoomId;
}

bool AGridActor::CheckIfCellInPlacedRoom(const FGridCell* Cell, FLinearColor& OutGridColor)
{
	if (Cell->RoomType != EGridRoomType::None)
	{
		const FGridRoom* Room = Rooms.Find(Cell->RoomId);
		if (Room)
		{
			OutGridColor = Room->GridColor;
			return true;
		}
	}

	return false;
}

void AGridActor::RebuildWalls()
{
	WallISM->ClearInstances();
	CreatedWallsPositions.Empty();

	const float Half = CellSize * 0.5f;

	for (auto& Pair : Rooms)
	{
		FGridRoom& Room = Pair.Value;

		// Get room borders
		int MinRow = INT_MAX, MaxRow = INT_MIN;
		int MinCol = INT_MAX, MaxCol = INT_MIN;

		for (FGridCell* Cell : Room.Cells)
		{
			if (!Cell) continue;
			if (Cell->Row < MinRow) MinRow = Cell->Row;
			if (Cell->Row > MaxRow) MaxRow = Cell->Row;
			if (Cell->Column < MinCol) MinCol = Cell->Column;
			if (Cell->Column > MaxCol) MaxCol = Cell->Column;
		}

		// Get doors at center of each side
		TSet<FIntPoint> DoorCells;
		DoorCells.Add(FIntPoint(MinRow, (MinCol + MaxCol) / 2)); // North
		DoorCells.Add(FIntPoint(MaxRow, (MinCol + MaxCol) / 2)); // South
		DoorCells.Add(FIntPoint((MinRow + MaxRow) / 2, MinCol)); // West
		DoorCells.Add(FIntPoint((MinRow + MaxRow) / 2, MaxCol)); // East
		
		for (FGridCell* Cell : Room.Cells)
		{
			if (!Cell) continue;

			int Row = Cell->Row;
			int Col = Cell->Column;
			
			TryAddWall(Cell, Row - 1, Col, EGridWallDirection::North, Half, DoorCells);
			TryAddWall(Cell, Row + 1, Col, EGridWallDirection::South, Half, DoorCells);
			TryAddWall(Cell, Row, Col - 1, EGridWallDirection::West, Half, DoorCells);
			TryAddWall(Cell, Row, Col + 1, EGridWallDirection::East, Half, DoorCells);
		}
	}
}

void AGridActor::TryAddWall(FGridCell* Cell, int NeighborRow, int NeighborCol, EGridWallDirection Dir, float Half, const TSet<FIntPoint>& DoorCells)
{
	FGridCell* Neighbor = GetGridCell(NeighborRow, NeighborCol);

	// Si voisin même salle : pas de mur
	if (Neighbor && Neighbor->RoomType == Cell->RoomType)
		return;

	FVector2D CellPos;
	if (!GetGridLocation(true, Cell->Row, Cell->Column, CellPos))
		return;

	FVector SpawnLoc(CellPos.X - GetActorLocation().X,
					 CellPos.Y - GetActorLocation().Y,
					 0);

	FRotator Rot = FRotator::ZeroRotator;

	switch (Dir)
	{
	case EGridWallDirection::North: SpawnLoc.X -= Half; break;
	case EGridWallDirection::South: SpawnLoc.X += Half; break;
	case EGridWallDirection::West:  SpawnLoc.Y -= Half; Rot = FRotator(0, 90, 0); break;
	case EGridWallDirection::East:  SpawnLoc.Y += Half; Rot = FRotator(0, 90, 0); break;
	}

	if (CreatedWallsPositions.Contains(SpawnLoc))
		return;

	// Passage auto si cellule = porte
	const bool bCreatePassage = DoorCells.Contains(FIntPoint(Cell->Row, Cell->Column));

	if (bCreatePassage)
	{
		constexpr float PassageHeight = 180.f;
		constexpr float WallHeight = 223.f;
		
		// Passage
		const FVector TopLoc = SpawnLoc + FVector(0, 0, PassageHeight + (WallHeight - PassageHeight) / 2);
		const FVector TopScale(1.f, 1.f, (WallHeight - PassageHeight) / WallHeight);
		WallISM->AddInstance(FTransform(Rot, TopLoc, TopScale));
	}
	else
	{
		// Mur
		WallISM->AddInstance(FTransform(Rot, SpawnLoc));
	}

	CreatedWallsPositions.Add(SpawnLoc);
}

bool AGridActor::GetRoomAtWorldLocation(const FVector& WorldLoc, FGridRoom*& OutRoom)
{
	int Row, Col;

	if (!GetCellAtLocation(WorldLoc, Row, Col))
		return false;

	FGridCell* Cell = GetGridCell(Row, Col);
	if (!Cell || Cell->RoomType == EGridRoomType::None)
		return false;

	OutRoom = Rooms.Find(Cell->RoomId);

	return OutRoom != nullptr;
}

void AGridActor::ShowGrid(bool bShow)
{
	LinesProceduralMesh->SetVisibility(bShow);

	for (auto& Pair : Cells)
	{
		FGridCell* Cell = &Pair.Value;
		if (!Cell)
			continue;

		Cell->CellProceduralMesh->SetVisibility(false);
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
			}
			else
			{
				Cell->CellProceduralMesh->SetVisibility(false); 
			}
		}
	}

	SelectedCells.Empty();
}

FGridCell* AGridActor::GetGridCell(int Row, int Column)
{
	return Cells.Find(FIntPoint(Row, Column));
}

void AGridActor::DeselectCell(int Row, int Column)
{
	const FGridCell* Cell = GetGridCell(Row, Column);
	if (Cell)
		Cell->CellProceduralMesh->SetVisibility(false);
}

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



