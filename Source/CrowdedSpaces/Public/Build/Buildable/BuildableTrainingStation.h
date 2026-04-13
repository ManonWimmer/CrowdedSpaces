#pragma once

#include "CoreMinimal.h"
#include "Build/BuildableObject.h"
#include "Selection/Selectable.h"
#include "BuildableTrainingStation.generated.h"

class UTrainingSubsystem;

UCLASS()
class CROWDEDSPACES_API ABuildableTrainingStation : public ABuildableObject, public ISelectable
{
	GENERATED_BODY()

public:
	ABuildableTrainingStation();

	virtual bool StartUsingImplementation(ANPC* NPC) override;
	virtual bool StopUsingImplementation(ANPC* NPC) override;

	// Selectable
	virtual void OnSelected() override;
	virtual void OnDeselected() override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY()
	TObjectPtr<UTrainingSubsystem> TrainingSubsystem{nullptr};
};
