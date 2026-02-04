#pragma once

#include "CoreMinimal.h"
#include "GridCellType.generated.h"

UENUM(BlueprintType)
enum class EGridCellType : uint8
{
	None			UMETA(DisplayName="None"),
	Generator		UMETA(DisplayName="Generator")
};
