#pragma once

#include "GridCellType.h"
#include "GridRoomType.h"
#include "ProceduralMeshComponent.h"
#include "GridCell.generated.h"

USTRUCT(Blueprintable)
struct FGridCell
{
	GENERATED_BODY()

	FGridCell(): Row(0), Column(0)
	{
	}

	FGridCell(
		int InRow,
		int InColumn,
		bool InOccupied,
		TObjectPtr<UProceduralMeshComponent> InMesh,
		TObjectPtr<UMaterialInstanceDynamic> InMaterial
	)
		:
	Row(InRow),
	Column(InColumn),
	bOccupied(InOccupied),
	CellProceduralMesh(InMesh),
	DynamicMaterial(InMaterial),
	CellType(EGridCellType::None),
	RoomType(EGridRoomType::None),
	RoomId(-1)
	{}
	
	int Row;
	int Column;
	
	bool bOccupied = false;

	UPROPERTY()
	EGridCellType CellType = EGridCellType::None;

	UPROPERTY()
	EGridRoomType RoomType = EGridRoomType::None;

	UPROPERTY()
	int RoomId = -1;

	UPROPERTY()
	TObjectPtr<UProceduralMeshComponent> CellProceduralMesh = nullptr;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterial = nullptr;
};