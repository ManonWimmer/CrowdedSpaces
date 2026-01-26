#include "AI/CustomBTTask.h"

void UCustomBTTask::StartAction() // Send current action to NPC, called on execute task
{
	if (!NPC)
		return;
	
	NPC->SetCurrentAction(NPCAction);
}

void UCustomBTTask::StopAction()  // Send idle action to NPC, called on task finished
{
	if (!NPC)
		return;

	NPC->SetCurrentAction(ENPCAction::Idle);
}
