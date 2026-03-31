#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StorageData.generated.h"

UCLASS(BlueprintType)
class CROWDEDSPACES_API UStorageData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float StorageRoomAddMaxMoneyPerCell = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float StorageRoomAddMaxFoodPerCell = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float StorageRoomAddMaxElectricityPerCell = 1000;
};
