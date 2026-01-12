#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Selectable.generated.h"

UINTERFACE(Blueprintable)
class USelectable : public UInterface
{
	GENERATED_BODY()
};


class CROWDEDSPACES_API ISelectable
{
	GENERATED_BODY()

public:
	// Sélection / désélection
	virtual void OnSelected() = 0;
	virtual void OnDeselected() = 0;

	// Infos à afficher
	virtual FString GetDisplayName() const = 0;
	virtual TMap<FString, FString> GetStats() const = 0;
};
