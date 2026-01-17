#pragma once

#include "CoreMinimal.h"
#include "MoralEventType.generated.h"

UENUM(BlueprintType)
enum class EMoralEventType : uint8
{
	NewNPC			UMETA(DisplayName="New NPC")
};
