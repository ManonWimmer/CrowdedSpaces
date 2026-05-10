#pragma once

#include "CoreMinimal.h"
#include "Action/Action.h"
#include "Action_NPC_StopAction.generated.h"

UCLASS()
class CROWDEDSPACES_API UAction_NPC_StopAction : public UAction
{
	GENERATED_BODY()

public:
	UAction_NPC_StopAction();

	virtual bool CanExecute_Implementation(AActor* Instigator) const override;
	virtual void Execute_Implementation(AActor* Instigator) override;
};
