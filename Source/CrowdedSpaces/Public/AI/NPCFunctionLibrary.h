#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NPCFunctionLibrary.generated.h"

class ANPC;

UCLASS()
class CROWDEDSPACES_API UNPCFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="NPC", meta=(WorldContext="WorldContextObject"))
	static ANPC* SpawnGlobalNPC(UObject* WorldContextObject);
};
