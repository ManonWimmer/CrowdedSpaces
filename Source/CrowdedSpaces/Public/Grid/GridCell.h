#pragma once

#include "GridCellType.h"
#include "GridRoomType.h"
#include "ProceduralMeshComponent.h"
#include "GridCell.generated.h"

USTRUCT(Blueprintable)
struct FGridCell
{
	GENERATED_BODY()

	int Row;
	int Column;
	
	bool bOccupied = false;

	UPROPERTY()
	EGridCellType CellType = EGridCellType::None;

	UPROPERTY()
	EGridRoomType RoomType = EGridRoomType::None;

	UPROPERTY()
	TObjectPtr<UProceduralMeshComponent> CellProceduralMesh = nullptr;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterial = nullptr;
};