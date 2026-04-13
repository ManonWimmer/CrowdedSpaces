#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TrainingData.generated.h"

USTRUCT(BlueprintType)
struct FTrainingEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "4"))
	int TrainingLevel; 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TrainingExpNeeded;
};

UCLASS(BlueprintType)
class CROWDEDSPACES_API UTrainingData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FTrainingEntry> TrainingData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AddExpPerHour = 10.f;
};
