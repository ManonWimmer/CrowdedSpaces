#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Build/BuildableObject.h"
#include "Grid/GridRoomType.h"
#include "BuildData.generated.h"

UCLASS(BlueprintType)
class CROWDEDSPACES_API UBuildData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Texture;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ABuildableObject> BuildClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MoneyCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int DestroyMoney;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int GridRowsX = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int GridColumnsY = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EGridRoomType RoomType = EGridRoomType::Any;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LoseElectricityPerHour = 10;
};
