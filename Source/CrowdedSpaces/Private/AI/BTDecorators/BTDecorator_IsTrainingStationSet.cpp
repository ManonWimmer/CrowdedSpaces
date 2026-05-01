#include "AI/BTDecorators/BTDecorator_IsTrainingStationSet.h"

#include "AIController.h"
#include "AI/NPC.h"
#include "AI/NPCController.h"
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
	const ANPCController* Controller = Cast<ANPCController>(OwnerComp.GetAIOwner());
	if (!Controller)
		return EBTNodeResult::Failed;
	
	const ANPC* NPC = Cast<ANPC>(Controller->GetPawn());
	if (!NPC)
		return EBTNodeResult::Failed;
	
	return NPC->HasTrainingStation();
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
