#include "AI/BTTasks/BTTask_ReserveBuildableSlot.h"

#include "AI/NPC.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Object/UsableObject.h"
#include "AIController.h"
#include "Build/SlotComponent.h"

UBTTask_ReserveBuildableSlot::UBTTask_ReserveBuildableSlot(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Reserve Buildable Slot";
}

EBTNodeResult::Type UBTTask_ReserveBuildableSlot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const TObjectPtr<AAIController> AI = OwnerComp.GetAIOwner();
	if (!AI)
		return EBTNodeResult::Failed;

	NPC = Cast<ANPC>(AI->GetPawn());
	if (!NPC)
		return EBTNodeResult::Failed;

	const TObjectPtr<UBlackboardComponent> Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard)
		return EBTNodeResult::Failed;

	const TObjectPtr<AUsableObject> Object = Cast<AUsableObject>(Blackboard->GetValueAsObject(TargetObjectKey.SelectedKeyName));

	const TObjectPtr<USlotComponent> Slot = Cast<USlotComponent>(Blackboard->GetValueAsObject(TargetSlotKey.SelectedKeyName));

	if (!Object || !Slot)
		return EBTNodeResult::Failed;

	// Already reserved
	if (NPC->GetCurrentObject() == Object)
		return EBTNodeResult::Succeeded;

	if (!Object->ReserveSpecificSlot(NPC, Slot))
		return EBTNodeResult::Failed;

	return EBTNodeResult::Succeeded;
}
