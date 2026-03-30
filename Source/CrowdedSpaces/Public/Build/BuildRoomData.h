#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Build/BuildableObject.h"
#include "Grid/GridRoomType.h"
#include "BuildRoomData.generated.h"

UCLASS(BlueprintType)
class CROWDEDSPACES_API UBuildRoomData : public UDataAsset
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
	int MoneyCost = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int DestroyMoney = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int GridRowsX = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int GridColumnsY = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EGridRoomType RoomType = EGridRoomType::Any;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor GridColor = FLinearColor::Yellow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LoseElectricityPerHour = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsUnlocked = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int UnlockWithNPCs = 1;
};
