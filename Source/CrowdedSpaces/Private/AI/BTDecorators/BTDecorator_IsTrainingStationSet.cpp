#include "AI/BTDecorators/BTDecorator_IsTrainingStationSet.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsTrainingStationSet::UBTDecorator_IsTrainingStationSet(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Is Training Station Set";

	bCreateNodeInstance = true; 
	bNotifyTick = true;
}

bool UBTDecorator_IsTrainingStationSet::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) const
{
	TObjectPtr<AAIController> AIController = OwnerComp.GetAIOwner();
	if (!AIController)
		return false;

	UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
	if (!Blackboard)
		return false;

	return Blackboard->GetValueAsObject("TrainingStation") != nullptr;
}

void UBTDecorator_IsTrainingStationSet::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const bool bCurrentValue = CalculateRawConditionValue(OwnerComp, NodeMemory);
	
	if (bCurrentValue != CachedValue)
	{
		CachedValue = bCurrentValue;
		OwnerComp.RequestExecution(this); 
	}
}
