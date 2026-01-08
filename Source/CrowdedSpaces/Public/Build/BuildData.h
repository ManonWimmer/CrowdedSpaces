#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
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
	UTexture2D* Texture;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MoneyCost;
};
