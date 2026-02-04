#include "Grid/GridActor.h"


AGridActor::AGridActor()
{
	LinesProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>("LinesProceduralMesh");
	CellsProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>("CellsProceduralMesh");

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	LinesProceduralMesh->SetupAttachment(RootComponent);
	CellsProceduralMesh->SetupAttachment(RootComponent);

	// ----- TEST INSTANCED MESH ----- //
	CellsISM = CreateDefaultSubobject<UInstancedStaticMeshComponent>("CellsISM");
	CellsISM->SetupAttachment(RootComponent);
	CellsISM->SetMobility(EComponentMobility::Movable);
	// ----- TEST INSTANCED MESH ----- //
	
	PrimaryActorTick.bCanEverTick = false;
}

void AGridActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	LinesProceduralMesh->ClearAllMeshSections();
	CellsProceduralMesh->ClearAllMeshSections();
	
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

	// ----- TEST POOL COMPONENTS ----- //
	/*
	for (int i = 0; i < Rows; i++)
	{
		for (int j = 0; j < Columns; j++)
		{
			UProceduralMeshComponent* OtherCellProceduralMesh = NewObject<UProceduralMeshComponent>(this);
			OtherCellProceduralMesh->RegisterComponent();
			OtherCellProceduralMesh->AttachToComponent(
				RootComponent,
				FAttachmentTransformRules::KeepRelativeTransform
			);

			FVector CellLocation(
                        i * CellSize,
                        j * CellSize,
                        0
                    );
            
                    OtherCellProceduralMesh->SetRelativeLocation(CellLocation);

			OtherCellProceduralMesh->SetVisibility(true);
			
			TArray<FVector> OtherCellVertices;
			TArray<int> OtherCellTriangles;
			float HalfCell = CellSize / 2;
			DrawLine(FVector(0, HalfCell, 0), FVector(CellSize, HalfCell, 0), CellSize, OtherCellVertices, OtherCellTriangles);
			TArray<FVector> OtherCellNormals;         
			TArray<FVector2D> OtherCellUV0;           
			TArray<FColor> OtherCellVertexColors;     
			TArray<FProcMeshTangent> OtherCellTangents;

			for (int x = 0; x < OtherCellVertices.Num(); x++)
			{
				OtherCellNormals.Add(FVector::UpVector); 
				OtherCellUV0.Add(FVector2D(0,0)); 
			}
	
			OtherCellProceduralMesh->CreateMeshSection(
				0,           
				OtherCellVertices,
				OtherCellTriangles,
				OtherCellNormals,
				OtherCellUV0,
				OtherCellVertexColors,
				OtherCellTangents,
				false         
			);

			FLinearColor RandomColor = FLinearColor::MakeRandomColor();

			TObjectPtr<UMaterialInstanceDynamic> CellsMaterialInstance =
				CreateMaterialInstance(RandomColor, CellOpacity);
			OtherCellProceduralMesh->SetMaterial(0, CellsMaterialInstance);
		}
	}
	*/
	// ----- TEST POOL COMPONENTS ----- //

	// ----- TEST INSTANCED MESH ----- //
	/*
	if (!CellMesh) return; 

	CellsISM->SetStaticMesh(CellMesh);

	// 3 floats pour RGB
	CellsISM->NumCustomDataFloats = 3;
	CellsISM->ClearInstances();

	const float MeshSize = 100.f; // plane UE par défaut = 100 cm
	const float Scale = CellSize / MeshSize;

	for (int Row = 0; Row < Rows; Row++)
	{
		for (int Column = 0; Column < Columns; Column++)
		{
			FVector Location(Column * CellSize + CellSize / 2, Row * CellSize + CellSize / 2, 0.f);

			FTransform InstanceTransform;
			InstanceTransform.SetLocation(Location);
			InstanceTransform.SetScale3D(FVector(Scale));

			int32 InstanceIndex = CellsISM->AddInstance(InstanceTransform);

			FLinearColor Color = FLinearColor::MakeRandomColor();

			CellsISM->SetCustomDataValue(InstanceIndex, 0, Color.R);
			CellsISM->SetCustomDataValue(InstanceIndex, 1, Color.G);
			CellsISM->SetCustomDataValue(InstanceIndex, 2, Color.B);
		}
	}
	*/
	// ----- TEST INSTANCED MESH ----- //

	// ----- TEST 1 PROCEDURAL FOR ALL CELLS ----- //
	 CellsProceduralMesh->ClearAllMeshSections();

    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UV0;
    TArray<FColor> VertexColors;
    TArray<FProcMeshTangent> Tangents;

    int32 VertexIndex = 0;

    for (int Row = 0; Row < Rows; Row++)
    {
        for (int Col = 0; Col < Columns; Col++)
        {
            FVector BaseLocation(Col * CellSize, Row * CellSize, 0.f);

            // 4 vertices du quad
            Vertices.Add(BaseLocation); // Bottom Left
            Vertices.Add(BaseLocation + FVector(CellSize, 0, 0)); // Bottom Right
            Vertices.Add(BaseLocation + FVector(0, CellSize, 0)); // Top Left
            Vertices.Add(BaseLocation + FVector(CellSize, CellSize, 0)); // Top Right

            // 2 triangles
            Triangles.Add(VertexIndex + 0);
            Triangles.Add(VertexIndex + 2);
            Triangles.Add(VertexIndex + 1);

            Triangles.Add(VertexIndex + 2);
            Triangles.Add(VertexIndex + 3);
            Triangles.Add(VertexIndex + 1);

            // Normals
            Normals.Add(FVector::UpVector);
            Normals.Add(FVector::UpVector);
            Normals.Add(FVector::UpVector);
            Normals.Add(FVector::UpVector);

            // UV
            UV0.Add(FVector2D(0, 0));
            UV0.Add(FVector2D(1, 0));
            UV0.Add(FVector2D(0, 1));
            UV0.Add(FVector2D(1, 1));

            // Vertex colors (random)
            FLinearColor RandomColor = FLinearColor::MakeRandomColor();
        	FColor VertexColor = RandomColor.ToFColor(true);
        	VertexColors.Add(VertexColor);
        	VertexColors.Add(VertexColor);
        	VertexColors.Add(VertexColor);
        	VertexColors.Add(VertexColor);

            // Tangents
            Tangents.Add(FProcMeshTangent(1, 0, 0));
            Tangents.Add(FProcMeshTangent(1, 0, 0));
            Tangents.Add(FProcMeshTangent(1, 0, 0));
            Tangents.Add(FProcMeshTangent(1, 0, 0));

            VertexIndex += 4;
        }
    }

    CellsProceduralMesh->CreateMeshSection(
        0,
        Vertices,
        Triangles,
        Normals,
        UV0,
        VertexColors,
        Tangents,
        false
    );
	
    CellsProceduralMesh->SetVisibility(true);
	// ----- TEST 1 PROCEDURAL FOR ALL CELLS ----- //
}

bool AGridActor::CheckIsValidCell(int Row, int Column)
{
	return (Row >= 0 && Row < Rows && Column >= 0 && Column < Columns);
}

bool AGridActor::GetCellAtLocation(FVector Location, int& OutRow, int& OutColumn)
{
	OutRow = FMath::FloorToInt(Rows * ((Location.X - GetActorLocation().X) / LineWidth()));
	OutColumn = FMath::FloorToInt(Columns * ((Location.Y - GetActorLocation().Y) / LineHeight()));

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



