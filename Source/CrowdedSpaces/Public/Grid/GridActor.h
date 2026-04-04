#pragma once

#include "CoreMinimal.h"
#include "GridCell.h"
#include "Grid/GridRoom.h"
#include "Grid/GridWallDirection.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Build/BuildRoomData.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "GridActor.generated.h"

struct FWallSegment
{
	TArray<FIntPoint> Cells;
	bool bHorizontal;
};

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
	void SetIsShowingRooms(const bool bShow) { bIsShowingRooms = bShow; }
	
	FGridCell* GetGridCell(int Row, int Column);
	
	int GetCellSize() const { return CellSize; }
	int GetRows() const { return Rows; }
	int GetColumns() const { return Columns; }

	UFUNCTION(BlueprintCallable, Category = "Grid")
	TMap<int, FGridRoom>& GetRooms() { return Rooms; }
	
	void SelectRoomCell(int Row, int Column, ERoomEditMode EditMode, EGridRoomType TargetRoomType);
	
	void ShowPlacedRooms(bool bShow);
	bool CheckIfCellInPlacedRoom(const FGridCell* Cell, FLinearColor& OutGridColor);

	UFUNCTION(BlueprintCallable, Category = "Room")
	bool DestroyRoom(int RoomId);

	UFUNCTION(BlueprintCallable)
	float GetRoomDestroyCost(int RoomId);

	// Walls
	void RebuildWalls();
	void CollectBorderCells(TArray<FIntPoint>& OutCells);
	bool IsBorder(const FGridCell* Cell, int R, int C);
	void BuildWallSegments(const TArray<FIntPoint>& CellsToCheck, TArray<FWallSegment>& OutSegments);
	void ExtractSegments(int Fixed, const TArray<int>& Values, bool bHorizontal, TArray<FWallSegment>& OutSegments);
	void SpawnWallWithDoorLogic(FGridCell* Cell, bool bHorizontal, float Half);
	void SpawnWallPerCell(FGridCell* GridCell, bool bHorizontal, float Half, bool bIsDoorCell);
	void SpawnSegment(const FWallSegment& Seg);
	
	FGridRoom* GetRoomAtCell(const FGridCell* Cell);
	bool GetRoomAtWorldLocation(const FVector& WorldLoc, FGridRoom*& OutRoom);
	FGridRoom* GetRoomOfSameType(EGridRoomType RoomType, int Row, int Col);
	FGridRoom* GetRoom(int RoomId);

	UFUNCTION(BlueprintCallable, Category = "Grid")
	void ShowGrid(bool bShow);
	
	const UBuildRoomData* GetRoomDataFromType(EGridRoomType RoomType) const;
	void RecomputeAllRooms();
	void AddCellsToRooms(TObjectPtr<UBuildRoomData> BuildData, TArray<FGridCell*> CellsToAdd);
	void RemoveCellsFromRooms(TArray<FGridCell*> CellsToRemove);

private:
	void DrawLine(const FVector& Start, const FVector& End, float Thickness, TArray<FVector>& Vertices, TArray<int>& Triangles);
	float LineWidth() const; 
	float LineHeight() const;
	
	TObjectPtr<UMaterialInstanceDynamic> CreateMaterialInstance(FLinearColor Color, float Opacity);
	
	UPROPERTY(EditAnywhere, Category = "Grid")
	
	TObjectPtr<UProceduralMeshComponent> LinesProceduralMesh;
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> LinesMaterialInstance = nullptr;

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

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInstancedStaticMeshComponent> WallISM;
	
	TSet<FVector> CreatedWallsPositions;

	UPROPERTY()
	TObjectPtr<UBuildSubsystem> BuildSubsystem{nullptr};

	TSet<FIntPoint> DoorCells;
	TSet<FIntPoint> DoorSegments;

	UPROPERTY()
	int MinWallSizeForDoors = 3;
};