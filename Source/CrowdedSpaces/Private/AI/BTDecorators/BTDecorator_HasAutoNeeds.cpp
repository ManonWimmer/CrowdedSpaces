#include "AI/BTDecorators/BTDecorator_HasAutoNeeds.h"

#include "AI/NPC.h"
#include "AI/NPCController.h"

UBTDecorator_HasAutoNeeds::UBTDecorator_HasAutoNeeds(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Has Auto Needs";

	bCreateNodeInstance = true; 
	bNotifyTick = true;
}

bool UBTDecorator_HasAutoNeeds::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const ANPCController* Controller = Cast<ANPCController>(OwnerComp.GetAIOwner());
	if (!Controller)
		return EBTNodeResult::Failed;

	const ANPC* NPC = Cast<ANPC>(Controller->GetPawn());
	if (!NPC)
		return EBTNodeResult::Failed;
	
	return NPC->HasAutoNeeds();
}

void UBTDecorator_HasAutoNeeds::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const bool bCurrentValue = CalculateRawConditionValue(OwnerComp, NodeMemory);
	
	if (bCurrentValue != CachedValue)
	{
		CachedValue = bCurrentValue;
		OwnerComp.RequestExecution(this); 
	}
}
