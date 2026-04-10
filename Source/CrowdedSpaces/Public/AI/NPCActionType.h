#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ENPCActionType : uint8
{
	Idle			UMETA(DisplayName="Idle"),
	Work			UMETA(DisplayName="Work"),
	Eat				UMETA(DisplayName="Eat"),
	Sleep			UMETA(DisplayName="Sleep"),

	MoveToWork		UMETA(DisplayName="Move To Work"),
	MoveToFood		UMETA(DisplayName="Move To Food"),
	MoveToBed		UMETA(DisplayName="Move To Bed"),

	SearchForWork	UMETA(DisplayName="Search For Work"),
	SearchForFood	UMETA(DisplayName="Search For Food"),
	SearchForBed	UMETA(DisplayName="Search For Bed"),
};
