#include "AI/BTTasks/BTTask_SetBlackboardBoolValue.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetBlackboardBoolValue::UBTTask_SetBlackboardBoolValue(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Set Blackboard Bool Value";
}

EBTNodeResult::Type UBTTask_SetBlackboardBoolValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TObjectPtr<UBlackboardComponent> Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard)
		return EBTNodeResult::Failed;

	Blackboard->SetValueAsBool(TargetBoolKey.SelectedKeyName, bNewBoolValue);

	return EBTNodeResult::Succeeded;
}
