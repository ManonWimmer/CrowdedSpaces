#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameSpeedType.h"
#include "TimeData.generated.h"

USTRUCT(BlueprintType)
struct FTimeDataStruct
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EGameSpeedType GameSpeedType = EGameSpeedType::Normal;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float GameSpeedMultiplier = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float GameTimeDilation = 1;
};

UCLASS(BlueprintType)
class CROWDEDSPACES_API UTimeData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FTimeDataStruct> TimeData;
};
