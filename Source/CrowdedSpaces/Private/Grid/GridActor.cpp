#include "Grid/GridActor.h"

AGridActor::AGridActor()
{
	LinesProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>("LinesProceduralMesh");
	CellsProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>("CellsProceduralMesh");

	LinesProceduralMesh->SetupAttachment(RootComponent);
	CellsProceduralMesh->SetupAttachment(RootComponent);
	
	PrimaryActorTick.bCanEverTick = true;
}

void AGridActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

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

	// Create line mesh
	TArray<FVector> Normals;         
	TArray<FVector2D> UV0;           
	TArray<FColor> VertexColors;     
	TArray<FProcMeshTangent> Tangents;
	
	LinesProceduralMesh->CreateMeshSection(
		0,           // section index
		LineVertices,
		LineTriangles,
		Normals,
		UV0,
		VertexColors,
		Tangents,
		false         // enable collision
	);
	
	
	TArray<FVector> CellVertices;
	TArray<int> CellTriangles;
}

void AGridActor::BeginPlay()
{
	Super::BeginPlay();
}

void AGridActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	Triangles.Add(StartIndex + 1);
	Triangles.Add(StartIndex + 2);

	Triangles.Add(StartIndex + 2);
	Triangles.Add(StartIndex + 1);
	Triangles.Add(StartIndex + 3);
}

float AGridActor::LineWidth() const
{
	return Rows * CellSize;
}

float AGridActor::LineHeight() const
{
	return Columns * CellSize;
}

