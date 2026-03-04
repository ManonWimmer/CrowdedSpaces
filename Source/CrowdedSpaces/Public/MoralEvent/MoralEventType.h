#pragma once

#include "CoreMinimal.h"
#include "MoralEventType.generated.h"

UENUM(BlueprintType)
enum class EMoralEventType : uint8
{
	DoNothing			UMETA(DisplayName="Do Nothing"),
	SpawnNPC			UMETA(DisplayName="Spawn NPC"),
	ExpelNPC			UMETA(DisplayName="Expel NPC"),
	Accept				UMETA(DisplayName="Accept"),
	LoseMoney			UMETA(DisplayName="Lose Money"),
	LoseFood 			UMETA(DisplayName="Lose Food")
};
