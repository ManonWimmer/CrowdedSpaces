#pragma once

#include "CoreMinimal.h"
#include "GridRoomType.generated.h"

UENUM(BlueprintType)
enum class EGridRoomType : uint8
{
	None			UMETA(DisplayName="None"),
	Diner			UMETA(DisplayName="Diner"),
	Generator		UMETA(DisplayName="Generator")
};
