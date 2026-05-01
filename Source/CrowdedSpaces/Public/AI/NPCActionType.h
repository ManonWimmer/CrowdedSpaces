#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ENPCActionType : uint8
{
	Idle						UMETA(DisplayName="Idle"),
	Work						UMETA(DisplayName="Work"),
	Eat							UMETA(DisplayName="Eat"),
	Sleep						UMETA(DisplayName="Sleep"),
	Train						UMETA(DisplayName="Train"),
	ExtinguishFire				UMETA(DisplayName="Extinguish Fire"),

	MoveToWork					UMETA(DisplayName="Move To Work"),
	MoveToFood					UMETA(DisplayName="Move To Food"),
	MoveToBed					UMETA(DisplayName="Move To Bed"),
	MoveToTrainingStation		UMETA(DisplayName="Move To Training Station"),
	MoveToFire					UMETA(DisplayName="Move To Fire"),

	SearchForWork				UMETA(DisplayName="Search For Work"),
	SearchForFood				UMETA(DisplayName="Search For Food"),
	SearchForBed				UMETA(DisplayName="Search For Bed"),
	SearchForTrainingStation	UMETA(DisplayName="Search For Training Station"),
};
