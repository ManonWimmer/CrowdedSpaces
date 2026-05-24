#pragma once

#include "CoreMinimal.h"
#include "Object/UsableObject.h"
#include "Selection/Selectable.h"
#include "BuildableHealingStation.generated.h"

UCLASS()
class CROWDEDSPACES_API ABuildableHealingStation : public AUsableObject, public ISelectable
{
	GENERATED_BODY()

public:
	ABuildableHealingStation();

	virtual bool StartUsingImplementation(ANPC* NPC) override;
	virtual bool StopUsingImplementation(ANPC* NPC) override;
	
	// Selectable
	virtual void OnSelected() override;
	virtual void OnDeselected() override;

	// Actions
	virtual void InitActions() override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
