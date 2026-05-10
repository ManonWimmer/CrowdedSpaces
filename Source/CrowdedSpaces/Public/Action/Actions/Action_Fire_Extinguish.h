#pragma once

#include "CoreMinimal.h"
#include "Action/Action.h"
#include "Action_Fire_Extinguish.generated.h"

UCLASS()
class CROWDEDSPACES_API UAction_Fire_Extinguish : public UAction
{
	GENERATED_BODY()

public:
	UAction_Fire_Extinguish();

	virtual bool CanExecute_Implementation(AActor* Instigator) const override;
	virtual void Execute_Implementation(AActor* Instigator) override;
};
