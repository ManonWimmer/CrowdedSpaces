#pragma once

#include "CoreMinimal.h"
#include "ProductionType.h"
#include "Engine/DataAsset.h"
#include "ProductionUpgradeData.generated.h"

USTRUCT(BlueprintType)
struct FUpgradeStruct
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 UpgradeResourcePerInterval = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float UpgradeProductionInterval = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 UpgradeCost = 100;
};

UCLASS(BlueprintType)
class CROWDEDSPACES_API UProductionUpgradeData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EProductionType ProductionType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 StartProductionInterval = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float StartResourcePerInterval = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FUpgradeStruct> UpgradesInOrder;
};


