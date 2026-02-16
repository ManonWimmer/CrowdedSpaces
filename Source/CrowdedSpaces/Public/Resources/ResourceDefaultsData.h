#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ResourceDefaultsData.generated.h"

enum class EResourceType : uint8;

USTRUCT(BlueprintType)
struct FResourceDefaults
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 Resource = 100;

	UPROPERTY(EditAnywhere)
	int32 MaxResource = 100;

	UPROPERTY(EditAnywhere)
	bool CanLoseAndRegenResource = false;

	UPROPERTY(EditAnywhere)
	int32 ResourceLossPerTick = 1;

	UPROPERTY(EditAnywhere)
	int32 ResourceRegenPerTick = 2;

	UPROPERTY(EditAnywhere)
	float TickInterval = 1.f;
};

UCLASS(BlueprintType)
class CROWDEDSPACES_API UResourceDefaultsData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TMap<EResourceType, FResourceDefaults> DefaultsByType;
};
