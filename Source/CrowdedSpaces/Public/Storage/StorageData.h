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
	float StorageRoomAddMaxMoney = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float StorageRoomAddMaxFood = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float StorageRoomAddMaxElectricity = 1000;
};
