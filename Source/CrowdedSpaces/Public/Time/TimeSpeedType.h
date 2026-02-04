#pragma once

#include "CoreMinimal.h"
#include "TimeSpeedType.generated.h"

UENUM(BlueprintType)
enum class ETimeSpeedType : uint8
{
	Paused			UMETA(DisplayName="Paused"),
	Normal			UMETA(DisplayName="Normal"),
	High			UMETA(DisplayName="High"),
	Ultra			UMETA(DisplayName="Ultra"),
};
