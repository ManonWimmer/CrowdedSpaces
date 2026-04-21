#include "AI/BTDecorators/BTDecorator_IsTrainPriority.h"

#include "AIController.h"
#include "AI/NPC.h"
#include "AI/NPCController.h"

UBTDecorator_IsTrainPriority::UBTDecorator_IsTrainPriority(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Is Train Priority";

	bCreateNodeInstance = true;
	bNotifyTick = true;
}

bool UBTDecorator_IsTrainPriority::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const ANPCController* Controller = Cast<ANPCController>(OwnerComp.GetAIOwner());
	if (!Controller)
		return EBTNodeResult::Failed;

	const ANPC* NPC = Cast<ANPC>(Controller->GetPawn());
	if (!NPC)
		return EBTNodeResult::Failed;
	
	return NPC->GetNPCPriorityType() == ENPCPriorityType::Train;
}

void UBTDecorator_IsTrainPriority::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const bool bCurrentValue = CalculateRawConditionValue(OwnerComp, NodeMemory);
	
	if (bCurrentValue != CachedValue)
	{
		CachedValue = bCurrentValue;
		OwnerComp.RequestExecution(this); 
	}
}
