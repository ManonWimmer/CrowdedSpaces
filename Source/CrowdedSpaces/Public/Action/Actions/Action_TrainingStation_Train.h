#pragma once

#include "CoreMinimal.h"
#include "Action/Action.h"
#include "Action_TrainingStation_Train.generated.h"

UCLASS()
class CROWDEDSPACES_API UAction_TrainingStation_Train : public UAction
{
	GENERATED_BODY()
	
public:
	UAction_TrainingStation_Train();

	virtual bool CanExecute_Implementation(AActor* Instigator) const override;
	virtual void Execute_Implementation(AActor* Instigator) override;
};
