#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType, Blueprintable)
enum class EGameModeState : uint8
{
	Game      UMETA(DisplayName="Game"),
	Building  UMETA(DisplayName="Building")
};
