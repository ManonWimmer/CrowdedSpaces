#pragma once

#include "CoreMinimal.h"
#include "Action/Action.h"
#include "Action_Generator_Work.generated.h"

UCLASS()
class CROWDEDSPACES_API UAction_Generator_Work : public UAction
{
	GENERATED_BODY()

public:
	UAction_Generator_Work();

	virtual bool CanExecute_Implementation(AActor* Instigator) const override;
	virtual void Execute_Implementation(AActor* Instigator) override;
};
