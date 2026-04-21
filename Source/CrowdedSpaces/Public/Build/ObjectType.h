#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EObjectType : uint8
{
	Default				UMETA(DisplayName="Default"),
	Generator			UMETA(DisplayName="Generator"),
	Food				UMETA(DisplayName="Food"),
	Bed					UMETA(DisplayName="Bed"),
	TrainingStation		UMETA(DisplayName="TrainingStation"),
};	
