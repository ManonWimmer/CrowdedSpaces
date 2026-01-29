#pragma once

#include "CoreMinimal.h"
#include "MoralEventType.generated.h"

UENUM(BlueprintType)
enum class EMoralEventType : uint8
{
	DoNothing			UMETA(DisplayName="Do Nothing"),
	SpawnNPC			UMETA(DisplayName="Spawn NPC"),
	ExpelNPC			UMETA(DisplayName="Expel NPC"),
};
