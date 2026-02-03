#include "Grid/GridActor.h"

AGridActor::AGridActor()
{
	LinesProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>("LinesProceduralMesh");
	CellsProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>("CellsProceduralMesh");

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	LinesProceduralMesh->SetupAttachment(RootComponent);
	CellsProceduralMesh->SetupAttachment(RootComponent);
	
	PrimaryActorTick.bCanEverTick = true;
}

void AGridActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
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

	// Draw cells
	TArray<FVector> CellsVertices;
	TArray<int> CellsTriangles;
	float HalfCell = CellSize / 2;
	DrawLine(FVector(0, HalfCell, 0), FVector(CellSize, HalfCell, 0), CellSize, CellsVertices, CellsTriangles);
	
	CellsProceduralMesh->SetVisibility(false);

	// Create cells mesh & material
	TArray<FVector> CellsNormals;         
	TArray<FVector2D> CellsUV0;           
	TArray<FColor> CellsVertexColors;     
	TArray<FProcMeshTangent> CellsTangents;

	for (int i = 0; i < CellsVertices.Num(); i++)
	{
		CellsNormals.Add(FVector::UpVector); 
		CellsUV0.Add(FVector2D(0,0)); 
	}
	
	CellsProceduralMesh->CreateMeshSection(
		0,           
		CellsVertices,
		CellsTriangles,
		CellsNormals,
		CellsUV0,
		CellsVertexColors,
		CellsTangents,
		false         
	);
	
	TObjectPtr<UMaterialInstanceDynamic> CellsMaterialInstance = CreateMaterialInstance(CellColor, CellOpacity);
	CellsProceduralMesh->SetMaterial(0, CellsMaterialInstance);
}

void AGridActor::BeginPlay()
{
	Super::BeginPlay();
}

void AGridActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AGridActor::CheckIsValidCell(int Row, int Column)
{
	return (Row >= 0 && Row < Rows && Column >= 0 && Column < Columns);
}

bool AGridActor::GetCellAtLocation(FVector Location, int& OutRow, int& OutColumn)
{
	OutRow = FMathf::Floor(Rows * ((Location.X - GetActorLocation().X) / LineWidth()));
	OutColumn = FMathf::Floor(Columns * ((Location.Y - GetActorLocation().Y) / LineHeight()));

	return CheckIsValidCell(OutRow, OutColumn);
}

bool AGridActor::GetGridLocation(bool bIsCenter, int Row, int Column, FVector2D& OutGridLocation)
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

void AGridActor::SelectCell(int Row, int Column)
{
	FVector2D GridLocation;
	if (!GetGridLocation(false, Row, Column, GridLocation))
	{
		CellsProceduralMesh->SetVisibility(false);
		return;
	}

	CellsProceduralMesh->SetVisibility(true);
	CellsProceduralMesh->SetWorldLocation(FVector(GridLocation.X, GridLocation.Y, GetActorLocation().Z));
}

void AGridActor::DeselectCell()
{
	CellsProceduralMesh->SetVisibility(false);
}

void AGridActor::DrawLine(FVector Start, FVector End, float Thickness, TArray<FVector>& Vertices, TArray<int>& Triangles)
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

TObjectPtr<UMaterialInstanceDynamic> AGridActor::CreateMaterialInstance(FLinearColor Color, float Opacity)
{
	if (!BaseMaterial)
	{
		UE_LOG(LogTemp, Warning, TEXT("BaseMaterial is null!"));
		return nullptr;
	}

	UMaterialInstanceDynamic* DynamicMat =
	   UMaterialInstanceDynamic::Create(BaseMaterial, this);

	if (DynamicMat)
	{
		DynamicMat->SetVectorParameterValue(TEXT("Color"), Color);
		DynamicMat->SetScalarParameterValue(TEXT("Opacity"), Opacity);
	}

	return DynamicMat;
}

