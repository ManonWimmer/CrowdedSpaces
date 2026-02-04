#include "Grid/GridActor.h"


AGridActor::AGridActor()
{
	LinesProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>("LinesProceduralMesh");

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	LinesProceduralMesh->SetupAttachment(RootComponent);
	
	PrimaryActorTick.bCanEverTick = false;
}

void AGridActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	#pragma region Create Lines
	LinesProceduralMesh->ClearAllMeshSections();
	
	TObjectPtr<UMaterialInstanceDynamic> CellMaterialInstance = CreateMaterialInstance(CellColor, CellOpacity);
	
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
	TArray<FColor> LinesVertexColors;     
	TArray<FProcMeshTangent> LinesTangents;

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

	TObjectPtr<UMaterialInstanceDynamic> LinesMaterialInstance = CreateMaterialInstance(LineColor, LineOpacity);
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
			float HalfCell = CellSize / 2;
			
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

void AGridActor::SelectCell(const int Row, const int Column)
{
	FGridCell* NewSelectedCell = GetGridCell(Row, Column);
	if (!NewSelectedCell)
		return;
	
	NewSelectedCell->CellProceduralMesh->SetVisibility(true);

	if (NewSelectedCell->bOccupied)
		NewSelectedCell->DynamicMaterial->SetVectorParameterValue(TEXT("Color"), FColor::Red);
	else
		NewSelectedCell->DynamicMaterial->SetVectorParameterValue(TEXT("Color"), FColor::Green);
	
	SelectedCells.Add(NewSelectedCell);
}

void AGridActor::DeselectSelectedCells()
{
	for (FGridCell* Cell : SelectedCells)
	{
		if (Cell)
			Cell->CellProceduralMesh->SetVisibility(false);
	}

	SelectedCells.Empty();
}

FGridCell* AGridActor::GetGridCell(int Row, int Column)
{
	return Cells.Find(FIntPoint(Row, Column));
}

void AGridActor::DrawLine(const FVector& Start, const FVector& End, const float Thickness, TArray<FVector>& Vertices, TArray<int>& Triangles)
{
	float HalfThickness = Thickness / 2;

	FVector Direction = End - Start;
	Direction.Normalize();
	Direction = FVector::CrossProduct(Direction, FVector(0,0,1));

	int StartIndex = Vertices.Num();
	Vertices.Add(Start + (Direction * HalfThickness)); // Top left corner
	Vertices.Add(End + (Direction * HalfThickness)); // Top right corner
	Vertices.Add(Start - (Direction * HalfThickness)); // Bottom left corner
	Vertices.Add(End - (Direction * HalfThickness)); // Bottom right corner

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



