#include "AI/CustomBTTask.h"
#include "Object/UsableObject.h"

void UCustomBTTask::StartAction(AUsableObject* Object) // Send current action to NPC, called on execute task
{
	if (!NPC)
		return;

	if (!Object)
		return;
	
	NPC->SetCurrentAction(Object->GetNPCUseAction());
}

void UCustomBTTask::StopAction()  // Send idle action to NPC, called on task finished
{
	if (!NPC)
		return;
	
	UE_LOG(LogTemp,Display,TEXT("NPC stop action"));
	
	NPC->SetCurrentAction(ENPCActionType::Idle);
}
