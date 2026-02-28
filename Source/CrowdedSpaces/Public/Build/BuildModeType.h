#pragma once

#include "CoreMinimal.h"
#include "BuildModeType.generated.h"

UENUM(BlueprintType)
enum class EBuildModeType : uint8
{
	Objects			UMETA(DisplayName="Objects"),
	Rooms			UMETA(DisplayName="Rooms")
};
