#pragma once

#include "CoreMinimal.h"

class UAction;

class CROWDEDSPACES_API IActionProvider
{
public:
	IActionProvider();
	virtual ~IActionProvider();
	
	virtual TArray<TObjectPtr<UAction>> GetAvailableActions(AActor* InInstigator) = 0;
	virtual void SetupActions() = 0;
};
