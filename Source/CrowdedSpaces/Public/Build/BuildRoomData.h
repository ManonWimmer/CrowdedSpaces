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
	TSubclassOf<ABuildableObject> BuildClass; // todo plus tard : buildable room ou rien ? 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MoneyCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int GridRowsX = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int GridColumnsY = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EGridRoomType RoomType = EGridRoomType::Any;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Room")
	FLinearColor GridColor = FLinearColor::Yellow;
};
