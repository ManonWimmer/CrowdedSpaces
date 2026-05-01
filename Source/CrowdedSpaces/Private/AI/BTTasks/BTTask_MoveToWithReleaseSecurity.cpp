#include "AI/BTTasks/BTTask_MoveToWithReleaseSecurity.h"

#include "AI/NPC.h"
#include "Build/BuildableObject.h"
#include "AIController.h"
#include "AI/NPCController.h"

UBTTask_MoveToWithReleaseSecurity::UBTTask_MoveToWithReleaseSecurity(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Move To (Release Security)";
}

EBTNodeResult::Type UBTTask_MoveToWithReleaseSecurity::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TObjectPtr<ANPCController> const Controller = Cast<ANPCController>(OwnerComp.GetAIOwner());
	if (!Controller)
		return EBTNodeResult::Failed;
	
	NPC = Cast<ANPC>(Controller->GetPawn());
    	if (!NPC)
    		return EBTNodeResult::Failed;

	const ABuildableObject* CurrentObject = NPC->GetCurrentObject();
	if (!CurrentObject)
		return EBTNodeResult::Failed;
	
	NPC->SetCurrentAction(CurrentObject->GetNPCMoveAction());
    		
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

EBTNodeResult::Type UBTTask_MoveToWithReleaseSecurity::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Cleanup(OwnerComp);

	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTTask_MoveToWithReleaseSecurity::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	if (TaskResult == EBTNodeResult::Failed)
		Cleanup(OwnerComp);

	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

void UBTTask_MoveToWithReleaseSecurity::Cleanup(const UBehaviorTreeComponent& OwnerComp) const
{
	if (!NPC)
		return;

	NPC->SetCurrentAction(ENPCActionType::Idle);

	ABuildableObject* CurrentObject = NPC->GetCurrentObject();
	if (!CurrentObject)
		return;

	// Release on failed move to
	CurrentObject->Release(NPC);
	
	if (NPC->GetActionObject() == CurrentObject)
		NPC->StopAction();
}
