#pragma once

#include "CoreMinimal.h"
#include "Action/Action.h"
#include "Action_HealingStation_Heal.generated.h"


UCLASS()
class CROWDEDSPACES_API UAction_HealingStation_Heal : public UAction
{
	GENERATED_BODY()

public:
	UAction_HealingStation_Heal();

	virtual bool CanExecute_Implementation(AActor* Instigator) const override;
	virtual void Execute_Implementation(AActor* Instigator) override;
};
