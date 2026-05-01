#include "AI/BTDecorators/BTDecorator_IsGeneratorSet.h"

#include "AIController.h"
#include "AI/NPC.h"
#include "AI/NPCController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsGeneratorSet::UBTDecorator_IsGeneratorSet(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Is Generator Set";

	bCreateNodeInstance = true; 
	bNotifyTick = true;
}

bool UBTDecorator_IsGeneratorSet::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const ANPCController* Controller = Cast<ANPCController>(OwnerComp.GetAIOwner());
	if (!Controller)
		return EBTNodeResult::Failed;
	
	const ANPC* NPC = Cast<ANPC>(Controller->GetPawn());
	if (!NPC)
		return EBTNodeResult::Failed;
	
	return NPC->HasGenerator();
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
