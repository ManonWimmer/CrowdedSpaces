#include "AI/BTTasks/BTTask_FindRandomLocation.h"

#include "NavigationSystem.h"
#include "AI/NPCController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Debug/CrowdedSpacesLogs.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find Random Location In NavMesh";

	bCreateNodeInstance = true; // Chaque NPC a sa propre instance    
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TObjectPtr<ANPCController> const Controller = Cast<ANPCController>(OwnerComp.GetAIOwner());
	if (!Controller)
	{
		CS_LOG("Cant find controller");
		return EBTNodeResult::Failed;
	}

	NPC = Cast<ANPC>(Controller->GetPawn());
	if (!NPC)
	{
		CS_LOG("Cant find npc");
		return EBTNodeResult::Failed;
	}
	
	FVector const Origin = NPC->GetActorLocation();
	TObjectPtr<UNavigationSystemV1> const NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavigationSystem)
	{
		CS_LOG("Cant find nav system");
		return EBTNodeResult::Failed;
	}
	
	FNavLocation Location;
	bool bFoundPoint = NavigationSystem->GetRandomPointInNavigableRadius(Origin, SearchRadius, Location);
	if (!bFoundPoint)
	{
		CS_LOG("Cant find point in nav mesh");
		return EBTNodeResult::Failed;
	}

	const TObjectPtr<UBlackboardComponent> Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard)
	{
		CS_LOG("Cant find blackboard");
		return EBTNodeResult::Failed;
	}
	
	Blackboard->SetValueAsVector(RandomLocationKey.SelectedKeyName, Location);
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

void UBTTask_FindRandomLocation::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
