#pragma once

#include "CoreMinimal.h"
#include "Build/BuildableObject.h"
#include "Selection/Selectable.h"
#include "BuildableFood.generated.h"

UCLASS()
class CROWDEDSPACES_API ABuildableFood : public ABuildableObject, public ISelectable
{
	GENERATED_BODY()

public:
	ABuildableFood();

	virtual bool StartUsingImplementation(UBTTask_UseBuildableObject* UseObjectTask) override;
	virtual bool StopUsingImplementation(UBTTask_UseBuildableObject* UseObjectTask) override;

	// Selectable
	virtual void OnSelected() override;
	virtual void OnDeselected() override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
