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


	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1,5,FColor::Yellow,"NPC stop action");
	UE_LOG(LogTemp,Display,TEXT("NPC stop action"));


	NPC->SetCurrentAction(ENPCActionWidget::Idle);
}
