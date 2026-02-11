#include "AI/CustomBTTask.h"
#include "Build/BuildableObject.h"

void UCustomBTTask::StartAction(ABuildableObject* Object) // Send current action to NPC, called on execute task
{
	if (!NPC)
		return;

	if (!Object)
		return;
	
	NPC->SetCurrentAction(Object->GetNPCAction());
}

void UCustomBTTask::StopAction()  // Send idle action to NPC, called on task finished
{
	if (!NPC)
		return;

	NPC->SetCurrentAction(ENPCAction::Idle);
}
