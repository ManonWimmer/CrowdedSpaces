#pragma once

#include "CoreMinimal.h"
#include "NPC.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "CustomBTTask.generated.h"

class AUsableObject;

UCLASS()
class CROWDEDSPACES_API UCustomBTTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	virtual void StartAction(AUsableObject* Object); // Send current action to NPC, called on execute task
	virtual void StopAction(); // Send idle action to NPC, called on task finished
	
	ANPC* GetNPC() const { return NPC; }

protected:
	UPROPERTY()
	TObjectPtr<ANPC> NPC = nullptr;
};
