#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EResourceType : uint8
{
	Money			UMETA(DisplayName="Money"),
	Electricity		UMETA(DisplayName="Electricity"),
	Food			UMETA(DisplayName="Food"),
	Energy			UMETA(DisplayName="Energy"),
	Oxygen			UMETA(DisplayName="Oxygen")
};
