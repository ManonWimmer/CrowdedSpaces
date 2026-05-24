#include "AI/BTTasks/BTTask_FindNearestAvailableBuildableObject.h"

#include "AI/NPCController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Build/BuildableRegistrySubsystem.h"
#include "Object/UsableObject.h"
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

	const ANPCController* Controller = Cast<ANPCController>(OwnerComp.GetAIOwner());
	if (!Controller)
		return EBTNodeResult::Failed;

	NPC = Cast<ANPC>(Controller->GetPawn());
	if (!NPC)
		return EBTNodeResult::Failed;

	NPC->SetCurrentAction(NPCSearchAction);

	const UWorld* World = NPC->GetWorld();
	if (!World)
		return EBTNodeResult::Failed;

	UBuildableRegistrySubsystem* BuildableRegistrySubsystem = World->GetSubsystem<UBuildableRegistrySubsystem>();
	if (!BuildableRegistrySubsystem)
		return EBTNodeResult::Failed;

	const FVector Origin = NPC->GetActorLocation();

	AUsableObject* BestObject = nullptr;
	USlotComponent* BestSlot = nullptr;
	float BestDist = FLT_MAX;

	for (TWeakObjectPtr<AUsableObject> Object : BuildableRegistrySubsystem->BuildableObjects)
	{
		if (!Object.IsValid())
			continue;

		if (Object->GetObjectType() != BuildableObjectType)
			continue;

		if (!Object->CanBeUsed())
			continue;

		if (Object->GetObjectType() == EObjectType::Generator)
		{
			const ABuildableGenerator* Generator = Cast<ABuildableGenerator>(Object.Get());
			if (!Generator)
				continue;
			
			if (Generator->GetProductionType() != NPC->GetWorkOnGeneratorType())
				continue;
		}

		if (!Object->IsAvailableForReservation(NPC))
			continue;

		USlotComponent* Slot = Object->GetNearestFreeAndWalkableSlot(NPC, Origin);
		if (!Slot)
			continue;

		const float Distance = FVector::Dist(Origin, Slot->GetComponentLocation());

		if (Distance < BestDist)
		{
			BestDist = Distance;
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