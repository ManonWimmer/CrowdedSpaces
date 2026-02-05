#pragma once

#include "GridRoomType.h"
#include "GridCell.h"
#include "GridRoom.generated.h"

USTRUCT(Blueprintable)
struct FGridRoom
{
	GENERATED_BODY()

	UPROPERTY()
	EGridRoomType RoomType = EGridRoomType::Any;

	UPROPERTY()
	int RoomId = -1;
	
	TArray<FGridCell*> Cells;

	UPROPERTY()
	FLinearColor GridColor = FLinearColor::Yellow;
};