#include "AI/BTDecorators/BTDecorator_IsActionObjectSet.h"

#include "AIController.h"
#include "AI/NPC.h"
#include "AI/NPCController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsActionObjectSet::UBTDecorator_IsActionObjectSet(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Is Action Object Set";

	bCreateNodeInstance = true; 
	bNotifyTick = true;
}

bool UBTDecorator_IsActionObjectSet::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const ANPCController* Controller = Cast<ANPCController>(OwnerComp.GetAIOwner());
	if (!Controller)
		return EBTNodeResult::Failed;
	
	const ANPC* NPC = Cast<ANPC>(Controller->GetPawn());
	if (!NPC)
		return EBTNodeResult::Failed;
	
	return NPC->HasActionObject();
}

void UBTDecorator_IsActionObjectSet::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const bool bCurrentValue = CalculateRawConditionValue(OwnerComp, NodeMemory);
	
	if (bCurrentValue != CachedValue)
	{
		CachedValue = bCurrentValue;
		OwnerComp.RequestExecution(this); 
	}
}
