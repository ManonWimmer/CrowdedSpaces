#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ENPCActionWidget : uint8
{
	Idle			UMETA(DisplayName="Idle"),
	Work			UMETA(DisplayName="Work"),
	Eat				UMETA(DisplayName="Eat"),
	Sleep			UMETA(DisplayName="Sleep")
};
