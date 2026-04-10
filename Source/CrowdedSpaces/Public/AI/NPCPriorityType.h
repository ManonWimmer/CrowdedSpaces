#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ENPCPriorityType : uint8
{
	Work			UMETA(DisplayName="Work"),
	Train			UMETA(DisplayName="Train"),
	HandleEvent		UMETA(DisplayName="Handle Event") // Peut-etre plus tard pour fire etc à gérer
};
