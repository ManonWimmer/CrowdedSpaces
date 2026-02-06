#pragma once

#include "CoreMinimal.h"
#include "GridCell.h"
#include "Grid/GridRoom.h"
#include "Grid/GridWallEdge.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Build/BuildRoomData.h"
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
	bool CheckIsValidCell(int Row, int Column) const;

	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool GetCellAtLocation(FVector Location, int& OutRow, int& OutColumn) const;
	
	UFUNCTION(BlueprintCallable, Category = "Grid")
	bool GetGridLocation(bool bIsCenter, int Row, int Column, FVector2D& OutGridLocation) const;

	UFUNCTION(BlueprintCallable, Category = "Grid")
	void SelectObjectCell(const int Row, const int Column, EGridRoomType RoomType);
	
	UFUNCTION(BlueprintCallable, Category = "Grid")
	void DeselectSelectedCells();

	UFUNCTION(BlueprintCallable, Category = "Grid")
	void SetIsShowingRooms(bool bShow) { bIsShowingRooms = bShow; }
	
	FGridCell* GetGridCell(int Row, int Column);
	
	int GetCellSize() const { return CellSize;}
	int GetRows() const { return Rows;}
	int GetColumns() const { return Columns;}
	
	void DeselectCell(int Row, int Column);
	void SelectRoomCell(int Row, int Column);
	
	void ShowPlacedRooms(bool bShow);
	int CreateRoom(const UBuildRoomData* BuildData, TArray<FGridCell*> CellsToAssign);
	bool CheckIfCellInPlacedRoom(const FGridCell* Cell, FLinearColor& OutGridColor);
	
	void SpawnWallsForRoom(const TArray<FGridCell*>& RoomCells);
	void TrySpawnWall(const int Row, const int Column, const FGridCell* OriginCell, FRotator Rotation);
	void ToggleEdge(const FGridWallEdge& Edge);
	void UpdateWallsForRoom(const TArray<FGridCell*>& RoomCells);
	void RebuildWalls();
	
private:
	void DrawLine(const FVector& Start, const FVector& End, float Thickness, TArray<FVector>& Vertices, TArray<int>& Triangles);
	float LineWidth() const; 
	float LineHeight() const;
	
	TObjectPtr<UMaterialInstanceDynamic> CreateMaterialInstance(FLinearColor Color, float Opacity);
	
	UPROPERTY(EditAnywhere, Category = "Grid")
	TObjectPtr<UProceduralMeshComponent> LinesProceduralMesh;

	UPROPERTY(EditAnywhere, Category="Grid")
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
	
	TMap<FIntPoint, FGridCell> Cells;
	TArray<FGridCell*> SelectedCells;

	int NextRoomId = 0;

	UPROPERTY()
	TMap<int, FGridRoom> Rooms; // id - room

	bool bIsShowingRooms = false;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> WallClass;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInstancedStaticMeshComponent> WallISM;

	TSet<FGridWallEdge> WallEdges;
};