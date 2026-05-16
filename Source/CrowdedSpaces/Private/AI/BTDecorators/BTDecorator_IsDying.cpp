#include "AI/BTDecorators/BTDecorator_IsDying.h"

#include "AI/NPC.h"
#include "AI/NPCController.h"

UBTDecorator_IsDying::UBTDecorator_IsDying(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Is Dying";

	bCreateNodeInstance = true; 
	bNotifyTick = true;
}

bool UBTDecorator_IsDying::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const ANPCController* Controller = Cast<ANPCController>(OwnerComp.GetAIOwner());
	if (!Controller)
		return EBTNodeResult::Failed;

	const ANPC* NPC = Cast<ANPC>(Controller->GetPawn());
	if (!NPC)
		return EBTNodeResult::Failed;
	
	return NPC->bIsInDieAnimation;
}

void UBTDecorator_IsDying::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const bool bCurrentValue = CalculateRawConditionValue(OwnerComp, NodeMemory);
	
	if (bCurrentValue != CachedValue)
	{
		CachedValue = bCurrentValue;
		OwnerComp.RequestExecution(this); 
	}
}