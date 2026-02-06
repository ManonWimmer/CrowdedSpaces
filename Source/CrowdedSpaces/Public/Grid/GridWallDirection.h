#pragma once

#include "CoreMinimal.h"
#include "GridWallDirection.generated.h"

UENUM()
enum class EGridWallDirection : uint8
{
	North,
	East,
	South,
	West
};