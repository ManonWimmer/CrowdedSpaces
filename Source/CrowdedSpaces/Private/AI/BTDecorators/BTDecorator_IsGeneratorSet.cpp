#include "AI/BTDecorators/BTDecorator_IsGeneratorSet.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsGeneratorSet::UBTDecorator_IsGeneratorSet(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Is Generator Set";

	bCreateNodeInstance = true; 
	bNotifyTick = true;
}

bool UBTDecorator_IsGeneratorSet::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	TObjectPtr<AAIController> AIController = OwnerComp.GetAIOwner();
	if (!AIController)
		return false;

	UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
	if (!Blackboard)
		return false;

	return Blackboard->GetValueAsObject("Generator") != nullptr;
}

void UBTDecorator_IsGeneratorSet::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const bool bCurrentValue = CalculateRawConditionValue(OwnerComp, NodeMemory);
	
	if (bCurrentValue != CachedValue)
	{
		CachedValue = bCurrentValue;
		OwnerComp.RequestExecution(this); 
	}
}
