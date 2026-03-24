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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Room")
	float LoseElectricityPerHour = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Room")
	bool bHasEnoughElectricity = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Room")
	bool bIsActivated = true;
};