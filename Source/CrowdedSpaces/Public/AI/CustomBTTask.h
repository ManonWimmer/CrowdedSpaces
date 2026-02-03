#pragma once

#include "CoreMinimal.h"
#include "NPC.h"
#include "NPCAction.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "CustomBTTask.generated.h"

UCLASS()
class CROWDEDSPACES_API UCustomBTTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	virtual void StartAction(); // Send current action to NPC, called on execute task
	virtual void StopAction(); // Send idle action to NPC, called on task finished

protected:
	ENPCAction NPCAction;

	UPROPERTY()
	TObjectPtr<ANPC> NPC = nullptr;
};
