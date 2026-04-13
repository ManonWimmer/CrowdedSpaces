#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TrainingData.generated.h"

UCLASS(BlueprintType)
class CROWDEDSPACES_API UTrainingData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<int, float> TrainingLevelAndExpNeeded;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AddExpPerHour = 10.f;
};
