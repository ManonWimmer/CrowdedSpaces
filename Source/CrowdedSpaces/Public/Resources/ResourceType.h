#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EResourceType : uint8
{
	None			UMETA(DisplayName="None"),
	Money			UMETA(DisplayName="Money"),
	Electricity		UMETA(DisplayName="Electricity"),
	Food			UMETA(DisplayName="Food"),
	Energy			UMETA(DisplayName="Energy"),
	Health			UMETA(DisplayName="Health"),
};
