#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EGameModeState : uint8
{
	Game      UMETA(DisplayName="Game"),
	Building  UMETA(DisplayName="Building")
};
