#pragma once

#include "CoreMinimal.h"
#include "RoomEditMode.generated.h"

UENUM(BlueprintType)
enum class ERoomEditMode : uint8
{
	Add				UMETA(DisplayName="Add"),
	Remove			UMETA(DisplayName="Remove")
};
