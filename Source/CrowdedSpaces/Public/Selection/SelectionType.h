#pragma once

#include "CoreMinimal.h"
#include "SelectionType.generated.h"

UENUM(BlueprintType)
enum class ESelectionType : uint8
{
	Default			UMETA(DisplayName="Default"),
	NPC				UMETA(DisplayName="NPC"),
	Generator		UMETA(DisplayName="Generator"),
	Room			UMETA(DisplayName="Room"),
	Food			UMETA(DisplayName="Food"),
	Bed				UMETA(DisplayName="Bed")
};
