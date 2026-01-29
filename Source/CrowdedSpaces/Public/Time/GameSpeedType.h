#pragma once

#include "CoreMinimal.h"
#include "GameSpeedType.generated.h"

UENUM(BlueprintType)
enum class EGameSpeedType : uint8
{
	Paused			UMETA(DisplayName="Paused"),
	Normal			UMETA(DisplayName="Normal"),
	High			UMETA(DisplayName="High"),
	Ultra			UMETA(DisplayName="Ultra"),
};
