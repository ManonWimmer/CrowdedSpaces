#pragma once

#include "CoreMinimal.h"

class UResourceComponent;
enum class EResourceType : uint8;

namespace PlayerHelpers
{
	UFUNCTION(BlueprintCallable, Category="Player", meta=(WorldContext="WorldContextObject"))
	UResourceComponent* GetPlayerResourceComponent(UWorld& World, const EResourceType ResourceType);
}

