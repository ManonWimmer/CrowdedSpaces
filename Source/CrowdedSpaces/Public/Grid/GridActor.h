#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "GridActor.generated.h"

UCLASS()
class CROWDEDSPACES_API AGridActor : public AActor
{
	GENERATED_BODY()

public:
	AGridActor();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool CheckIsValidCell(int Row, int Column);

	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool GetCellAtLocation(FVector Location, int& OutRow, int& OutColumn);
	
	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool GetGridLocation(bool bIsCenter, int Row, int Column, FVector2D& OutGridLocation);

	UFUNCTION(BlueprintCallable, Category = "Grid")
	void SelectCell(int Row, int Column);
	
	UFUNCTION(BlueprintCallable, Category = "Grid")
	void DeselectCell();

private:
	void DrawLine(FVector Start, FVector End, float Thickness, TArray<FVector>& Vertices, TArray<int>& Triangles);
	float LineWidth() const; 
	float LineHeight() const;
	TObjectPtr<UMaterialInstanceDynamic> CreateMaterialInstance(FLinearColor Color, float Opacity);
	
	UPROPERTY(EditAnywhere, Category = "Grid")
	TObjectPtr<UProceduralMeshComponent> LinesProceduralMesh;

	UPROPERTY(EditAnywhere, Category = "Grid")
	TObjectPtr<UProceduralMeshComponent> CellsProceduralMesh;

	UPROPERTY(EditAnywhere, Category="Materials")
	TObjectPtr<UMaterialInterface> BaseMaterial;

	UPROPERTY(EditAnywhere, Category = "Grid")
	int Rows = 10;

	UPROPERTY(EditAnywhere, Category = "Grid")
	int Columns = 10;

	UPROPERTY(EditAnywhere, Category = "Grid")
	float CellSize = 100.f;

	UPROPERTY(EditAnywhere, Category = "Grid")
	float LineThickness = 10.f;

	UPROPERTY(EditAnywhere, Category = "Grid")
	FLinearColor LineColor;

	UPROPERTY(EditAnywhere, Category = "Grid")
	FLinearColor CellColor;

	UPROPERTY(EditAnywhere, Category = "Grid")
	float LineOpacity = 1.f;

	UPROPERTY(EditAnywhere, Category = "Grid")
	float CellOpacity = 0.25f;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> CellMesh; // Plane

	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> CellsISM;

	// ----- TEST CREATE IN TICK ----- //
	TArray<FVector> PendingVertices;
	TArray<int32> PendingTriangles;
	TArray<FVector> PendingNormals;
	TArray<FVector2D> PendingUV0;
	TArray<FColor> PendingVertexColors;
	TArray<FProcMeshTangent> PendingTangents;

	int32 PendingVertexIndex = 0;
	int32 CurrentRow = 0;
	int32 CurrentCol = 0;
	int32 CellsPerTick = 5; 

	bool bGeneratingCells = false;
	// ----- TEST CREATE IN TICK ----- //
};