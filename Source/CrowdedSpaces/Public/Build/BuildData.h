#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Object/UsableObject.h"
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
	TSubclassOf<AUsableObject> BuildClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EGridRoomType RoomType = EGridRoomType::Any;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int GridRowsX = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int GridColumnsY = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MoneyCost = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int DestroyMoney = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LoseElectricityPerHour = 10;
};
