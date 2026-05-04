#pragma once

#include "CoreMinimal.h"
#include "Action/Action.h"
#include "Action_Object_Repair.generated.h"

UCLASS()
class CROWDEDSPACES_API UAction_Object_Repair : public UAction
{
	GENERATED_BODY()

public:
	UAction_Object_Repair();

	virtual bool CanExecute_Implementation(AActor* Instigator) const override;
	virtual void Execute_Implementation(AActor* Instigator) override;
};
