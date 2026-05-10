#include "AI/BTTasks/BTTask_ReserveBuildableSlot.h"

#include "AI/NPC.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Build/BuildableObject.h"
#include "AIController.h"
#include "Build/SlotComponent.h"

UBTTask_ReserveBuildableSlot::UBTTask_ReserveBuildableSlot(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Reserve Buildable Slot";
}

EBTNodeResult::Type UBTTask_ReserveBuildableSlot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* AI = OwnerComp.GetAIOwner();
	if (!AI)
		return EBTNodeResult::Failed;

	NPC = Cast<ANPC>(AI->GetPawn());
	if (!NPC)
		return EBTNodeResult::Failed;

	const UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard)
		return EBTNodeResult::Failed;

	ABuildableObject* Object = Cast<ABuildableObject>(Blackboard->GetValueAsObject(TargetObjectKey.SelectedKeyName));

	USlotComponent* Slot = Cast<USlotComponent>(Blackboard->GetValueAsObject(TargetSlotKey.SelectedKeyName));

	if (!Object || !Slot)
		return EBTNodeResult::Failed;

	// Already reserved
	if (NPC->GetCurrentObject() == Object)
		return EBTNodeResult::Succeeded;

	if (!Object->ReserveSpecificSlot(NPC, Slot))
		return EBTNodeResult::Failed;

	return EBTNodeResult::Succeeded;
}
