#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NameList.generated.h"


UCLASS()
class CROWDEDSPACES_API UNameList : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Names")
	TArray<FString> FirstNames;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Names")
	TArray<FString> LastNames;
};
