#include "AI/BTTasks/BTTask_UseBuildableObject.h"

#include "AI/NPCController.h"
#include "Build/BuildableObject.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_UseBuildableObject::UBTTask_UseBuildableObject(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Use Buildable Object";

	bCreateNodeInstance = true;  
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_UseBuildableObject::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TObjectPtr<ANPCController> Controller = Cast<ANPCController>(OwnerComp.GetAIOwner());
	if (!Controller)
		return EBTNodeResult::Failed;

	NPC = Cast<ANPC>(Controller->GetPawn());
	if (!NPC)
		return EBTNodeResult::Failed;
	
	TObjectPtr<UBlackboardComponent> Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard)
		return EBTNodeResult::Failed;

	TObjectPtr<ABuildableObject> TargetObject = Cast<ABuildableObject>(Blackboard->GetValueAsObject(TargetObjectKey.SelectedKeyName));
	if (!TargetObject)
		return EBTNodeResult::Failed;

	if (!TargetObject->CanBeUsed())
		return EBTNodeResult::Failed;

	StartAction(TargetObject);

	TargetObject->StartUsing(NPC);
	if (!TargetObject->StartUsingImplementation(this))
	{
		return EBTNodeResult::Failed;
	}

	OwnerCompPtr = &OwnerComp;
	
	return EBTNodeResult::InProgress;
}

void UBTTask_UseBuildableObject::OnStopAction()
{
	if (!OwnerCompPtr.IsValid())
		return;

	TObjectPtr<ANPCController> Controller = Cast<ANPCController>(OwnerCompPtr->GetAIOwner());
	if (!Controller)
		return;

	NPC = Cast<ANPC>(Controller->GetPawn());
	if (!NPC)
		return;
	
	FinishLatentTask(*OwnerCompPtr, EBTNodeResult::Succeeded);
}

void UBTTask_UseBuildableObject::OnTargetDestroyed() const
{
	OwnerCompPtr->GetBlackboardComponent()->SetValueAsObject(TargetObjectKey.SelectedKeyName, nullptr);
	ForceStopTask();
}

void UBTTask_UseBuildableObject::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                                EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
	
	TObjectPtr<UBlackboardComponent> Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard)
		return;

	StopAction();
	
	TObjectPtr<ABuildableObject> TargetObject = Cast<ABuildableObject>(Blackboard->GetValueAsObject(TargetObjectKey.SelectedKeyName));
	if (!TargetObject)
		return;

	TargetObject->StopUsingImplementation(this);
	TargetObject->StopUsing(NPC);

	Blackboard->SetValueAsObject(TargetObjectKey.SelectedKeyName, nullptr);
}

void UBTTask_UseBuildableObject::ForceStopTask() const
{
	FinishLatentTask(*OwnerCompPtr, EBTNodeResult::Failed);
}
