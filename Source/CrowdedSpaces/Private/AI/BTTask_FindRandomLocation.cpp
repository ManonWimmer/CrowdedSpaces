#include "AI/BTTask_FindRandomLocation.h"

#include "NavigationSystem.h"
#include "AI/NPCController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find Random Location In NavMesh";
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ANPCController* const Controller = Cast<ANPCController>(OwnerComp.GetAIOwner());
	if (!Controller)
		return EBTNodeResult::Failed;

	APawn* const NPC = Controller->GetPawn();
	if (!NPC)
		return EBTNodeResult::Failed;
	
	// Find random location in navigation system
	FVector const Origin = NPC->GetActorLocation();
	if (UNavigationSystemV1* const NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
	{
		FNavLocation Location;
		if (NavigationSystem->GetRandomPointInNavigableRadius(Origin, SearchRadius, Location))
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Location);	
		}

		// Success
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
