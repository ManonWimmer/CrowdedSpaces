#include "AI/BTTasks/BTTask_FindNearestAvailableBuildableObject.h"

#include "AI/NPCController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Build/BuildableRegistrySubsystem.h"
#include "Build/BuildableObject.h"
#include "Build/SlotComponent.h"

UBTTask_FindNearestAvailableBuildableObject::UBTTask_FindNearestAvailableBuildableObject(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find Nearest Available Buildable Object Location In NavMesh";
}

EBTNodeResult::Type UBTTask_FindNearestAvailableBuildableObject::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard)
		return EBTNodeResult::Failed;

	ANPCController* Controller = Cast<ANPCController>(OwnerComp.GetAIOwner());
	if (!Controller)
		return EBTNodeResult::Failed;

	NPC = Cast<ANPC>(Controller->GetPawn());
	if (!NPC)
		return EBTNodeResult::Failed;

	UWorld* World = NPC->GetWorld();
	if (!World)
		return EBTNodeResult::Failed;

	UBuildableRegistrySubsystem* BRS = World->GetSubsystem<UBuildableRegistrySubsystem>();
	if (!BRS)
		return EBTNodeResult::Failed;

	FVector Origin = NPC->GetActorLocation();

	ABuildableObject* BestObject = nullptr;
	USlotComponent* BestSlot = nullptr;
	float BestDist = FLT_MAX;

	for (TWeakObjectPtr<ABuildableObject> Object : BRS->BuildableObjects)
	{
		if (!Object.IsValid())
			continue;

		if (Object->GetObjectType() != BuildableObjectType)
			continue;

		if (!Object->CanBeUsed())
			continue;

		if (Object->GetObjectType() == EObjectType::Generator)
		{
			ABuildableGenerator* Generator = Cast<ABuildableGenerator>(Object.Get());
			if (!Generator)
				continue;
			
			if (Generator->GetProductionType() != NPC->GetWorkOnGeneratorType())
				continue;
		}

		if (!Object->IsAvailableForReservation(NPC))
			continue;

		USlotComponent* Slot = Object->GetNearestFreeSlot(Origin);
		if (!Slot)
			continue;

		float Dist = FVector::Dist(Origin, Slot->GetComponentLocation());

		if (Dist < BestDist)
		{
			BestDist = Dist;
			BestObject = Object.Get();
			BestSlot = Slot;
		}
	}

	if (!BestObject || !BestSlot)
		return EBTNodeResult::Failed;

	Blackboard->SetValueAsObject(TargetObjectKey.SelectedKeyName, BestObject);
	Blackboard->SetValueAsVector(TargetLocationKey.SelectedKeyName, BestSlot->GetComponentLocation());
	Blackboard->SetValueAsObject(TargetSlotKey.SelectedKeyName, BestSlot);

	return EBTNodeResult::Succeeded;
}