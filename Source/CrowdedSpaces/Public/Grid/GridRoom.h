#pragma once

#include "GridRoomType.h"
#include "GridCell.h"
#include "GridRoom.generated.h"

USTRUCT(Blueprintable)
struct FGridRoom
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Room")
	EGridRoomType RoomType = EGridRoomType::Any;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Room")
	int RoomId = -1;
	
	TArray<FGridCell*> Cells;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Room")
	FLinearColor GridColor = FLinearColor::Yellow;
};