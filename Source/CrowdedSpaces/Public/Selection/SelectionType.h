#pragma once

#include "CoreMinimal.h"
#include "SelectionType.generated.h"

UENUM(BlueprintType)
enum class ESelectionType : uint8
{
	Default				UMETA(DisplayName="Default"),
	NPC					UMETA(DisplayName="NPC"),
		
	Generator			UMETA(DisplayName="Generator"),
	Food				UMETA(DisplayName="Food"),
	Bed					UMETA(DisplayName="Bed"),
	TrainingStation 	UMETA(DisplayName="Training Station"),
	HealingStation		UMETA(DisplayName="Healing Station"),
		
	Room				UMETA(DisplayName="Room"),
	StorageRoom			UMETA(DisplayName="StorageRoom"),
};
