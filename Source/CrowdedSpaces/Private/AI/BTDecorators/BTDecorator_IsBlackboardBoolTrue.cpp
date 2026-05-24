#include "AI/BTDecorators/BTDecorator_IsBlackboardBoolTrue.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsBlackboardBoolTrue::UBTDecorator_IsBlackboardBoolTrue(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Is Blackboard Bool True";

	bCreateNodeInstance = true; 
	bNotifyTick = true;
}

bool UBTDecorator_IsBlackboardBoolTrue::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const TObjectPtr<UBlackboardComponent> Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard)
		return EBTNodeResult::Failed;
	
	return Blackboard->GetValueAsBool(BoolKeyToCheck.SelectedKeyName);
}

void UBTDecorator_IsBlackboardBoolTrue::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const bool bCurrentValue = CalculateRawConditionValue(OwnerComp, NodeMemory);
	
	if (bCurrentValue != CachedValue)
	{
		CachedValue = bCurrentValue;
		OwnerComp.RequestExecution(this); 
	}
}
