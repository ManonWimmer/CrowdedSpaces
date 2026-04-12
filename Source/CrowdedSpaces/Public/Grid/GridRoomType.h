#pragma once

#include "CoreMinimal.h"
#include "GridRoomType.generated.h"

UENUM(BlueprintType)
enum class EGridRoomType : uint8
{
	None			UMETA(DisplayName="None"),
	Any				UMETA(DisplayName="Any"),
	Diner			UMETA(DisplayName="Diner"),
	Generator		UMETA(DisplayName="Generator"),
	Bedroom			UMETA(DisplayName="Bedroom"),
	Storage			UMETA(DisplayName="Storage"),
	Training		UMETA(DisplayName="Training"),
};
