#pragma once

#include "CoreMinimal.h"
#include "Action/Action.h"
#include "Action_NPC_Select.generated.h"

UCLASS()
class CROWDEDSPACES_API UAction_NPC_Select : public UAction
{
	GENERATED_BODY()

public:
	UAction_NPC_Select();

	virtual bool CanExecute_Implementation(AActor* Instigator) const override;
	virtual void Execute_Implementation(AActor* Instigator) override;
};
