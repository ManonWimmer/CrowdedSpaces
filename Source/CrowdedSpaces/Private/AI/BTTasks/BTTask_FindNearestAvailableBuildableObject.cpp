#include "AI/BTTasks/BTTask_FindNearestAvailableBuildableObject.h"

#include "AI/NPCController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Build/BuildableRegistrySubsystem.h"

UBTTask_FindNearestAvailableBuildableObject::UBTTask_FindNearestAvailableBuildableObject(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find Nearest Available Buildable Object Location In NavMesh";
	
	bCreateNodeInstance = true;  
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_FindNearestAvailableBuildableObject::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (BuildableObjectType == EObjectType::Default)
		return EBTNodeResult::Failed;
	
	TObjectPtr<ANPCController> const Controller = Cast<ANPCController>(OwnerComp.GetAIOwner());
	if (!Controller)
		return EBTNodeResult::Failed;

	NPC = Cast<ANPC>(Controller->GetPawn());
	if (!NPC)
		return EBTNodeResult::Failed;

	TObjectPtr<UWorld> World = NPC->GetWorld();
	if (!World)
		return EBTNodeResult::Failed;
	
	FVector const Origin = NPC->GetActorLocation();
	ABuildableObject* NearestAvailableObject = nullptr;
	float NearestDistance = 0.0f;

	TObjectPtr<UBuildableRegistrySubsystem> BRS = World->GetSubsystem<UBuildableRegistrySubsystem>();
	if (!BRS)
		return EBTNodeResult::Failed;

	// Get nearest available buildable object
	for (TWeakObjectPtr<ABuildableObject> Object : BRS->BuildableObjects) 
	{
		if (!Object.IsValid())
			continue; 
		
		if (Object->GetObjectType() != BuildableObjectType)
			continue;
		
		if (Object->HasNPCUsing())
			continue;

		if (!Object->CanBeUsed())
			continue;

		if (Object->IsReservedByOther(NPC))
			continue;

		// Check same generator type as npc should work on
		if (BuildableObjectType == EObjectType::Generator)
		{
			ABuildableGenerator* Generator = Cast<ABuildableGenerator>(Object);
			if (NPC->GetWorkOnGeneratorType() != Generator->GetProductionType())
				continue;
		}
		
		float Distance = FVector::Distance(Origin, Object->GetActorLocation());
		if (Distance < SearchRadius / 2)
		{
			if (NearestAvailableObject)
			{
				if (Distance < NearestDistance)
				{
					NearestAvailableObject = Object.Get();
					NearestDistance = Distance;
				}
			}
			else
			{
				NearestAvailableObject = Object.Get();
				NearestDistance = Distance;
			}
		}
	}

	// Success or Failed + set keys
	if (NearestAvailableObject)
	{
		TObjectPtr<UBlackboardComponent> Blackboard = OwnerComp.GetBlackboardComponent();
		if (!Blackboard)
			return EBTNodeResult::Failed;

		if (!NearestAvailableObject->TryReserve(NPC))
			return EBTNodeResult::Failed;
		
		Blackboard->SetValueAsVector(TargetLocationKey.SelectedKeyName, NearestAvailableObject->GetActorLocation());
		Blackboard->SetValueAsObject(TargetObjectKey.SelectedKeyName, NearestAvailableObject);
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	else
	{
		return EBTNodeResult::Failed;
	}
}

void UBTTask_FindNearestAvailableBuildableObject::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
