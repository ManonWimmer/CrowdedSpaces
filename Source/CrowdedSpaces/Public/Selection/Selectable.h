#pragma once

#include "CoreMinimal.h"
#include "SelectionType.h"
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
	
	virtual FString GetDisplayName() const = 0;
	virtual TObjectPtr<AActor> GetSelectableActor() = 0;

	ESelectionType SelectionType = ESelectionType::Default;
};
