#pragma once

#include "CoreMinimal.h"
#include "ProductionType.generated.h"

UENUM(BlueprintType)
enum class EProductionType : uint8
{
	Money			UMETA(DisplayName="Money"),
	Food			UMETA(DisplayName="Food"),
	Electricity		UMETA(DisplayName="Electricity"),
};
