#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TimeSpeedType.h"
#include "MoralEvent/MoralEvent.h"
#include "TimeData.generated.h"

UENUM(BlueprintType)
enum class ETimeType : uint8
{
	AM,
	PM
};

USTRUCT(BlueprintType)
struct FTimeDataStruct
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ETimeSpeedType TimeSpeedType = ETimeSpeedType::Normal;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float GameSpeedMultiplier = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float GameTimeDilation = 1;
};

USTRUCT(BlueprintType)
struct FMoralEventProbabilitiesDataStruct
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UMoralEvent> MoralEvent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Probability = 1;
};

USTRUCT(BlueprintType)
struct FMoralEventsDaysProbabilitiesDataStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Title = TEXT("Day 1 AM");
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Day = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ETimeType TimeType = ETimeType::AM;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FMoralEventProbabilitiesDataStruct> MoralEventsDaysProbabilities;
};

UCLASS(BlueprintType)
class CROWDEDSPACES_API UTimeData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FTimeDataStruct> TimeData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MoralEventHourAM;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MoralEventHourPM;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (TitleProperty = "Title"))
	TArray<FMoralEventsDaysProbabilitiesDataStruct> MoralEventDaysProbabilities;
};
