#pragma once

#include "CoreMinimal.h"
#include "MoralEventChoice.generated.h"

UENUM(BlueprintType)
enum class EMoralEventChoice : uint8
{
	DoNothing			UMETA(DisplayName="Do Nothing"),
	SpawnNPC			UMETA(DisplayName="Spawn NPC"),
};
