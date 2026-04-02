#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Resources/ResourceType.h"
#include "PlayerFunctionLibrary.generated.h"

class UResourceComponent;

UCLASS()
class CROWDEDSPACES_API UPlayerFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Player", meta=(WorldContext="WorldContextObject"))
	static UResourceComponent* GetPlayerResourceComponent(UObject* WorldContextObject, const EResourceType ResourceType);
};
