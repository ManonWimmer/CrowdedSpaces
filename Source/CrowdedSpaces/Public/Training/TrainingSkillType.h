#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ETrainingSkillType : uint8
{
	MoneyProduction			UMETA(DisplayName="Money Production"),
	FoodProduction			UMETA(DisplayName="Food Production"),
	ElectricityProduction	UMETA(DisplayName="Electricity Production")
};
