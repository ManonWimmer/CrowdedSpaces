#include "AI/BTTasks/BTTask_FindNearestAvailableFood.h"

#include "EngineUtils.h"
#include "AI/NPCController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Build/Buildable/BuildableFood.h"

UBTTask_FindNearestAvailableFood::UBTTask_FindNearestAvailableFood(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find Nearest Available Food Location In NavMesh";
	
	bCreateNodeInstance = true; // Chaque NPC a sa propre instance    
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_FindNearestAvailableFood::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ANPCController* const Controller = Cast<ANPCController>(OwnerComp.GetAIOwner());
	if (!Controller)
		return EBTNodeResult::Failed;

	NPC = Cast<ANPC>(Controller->GetPawn());
	if (!NPC)
		return EBTNodeResult::Failed;

	UWorld* World = NPC->GetWorld();
	if (!World)
		return EBTNodeResult::Failed;

	FVector const Origin = NPC->GetActorLocation();
	ABuildableFood* NearestAvailableFood = nullptr;
	float NearestDistance = 0.0f;
	
	for (TActorIterator<ABuildableFood> It(World); It; ++It) // TObjectIterator marchait pas en renvoyait tjrs en actor location 0,0,-78
	{
		ABuildableFood* Food = *It;
		if (!Food || !IsValid(Food))
			continue; 
		
		// Get nearest food AVAILABLE
		if (!Food->IsAvailable())
			continue;
		
		float Distance = FVector::Distance(Origin, Food->GetActorLocation());
		if (Distance < SearchRadius / 2)
		{
			if (NearestAvailableFood)
			{
				if (Distance < NearestDistance)
				{
					NearestAvailableFood = Food;
					NearestDistance = Distance;
				}
			}
			else
			{
				NearestAvailableFood = Food;
				NearestDistance = Distance;
			}
		}
	}

	// Success or Failed + set keys
	if (NearestAvailableFood)
	{
		UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
		if (!Blackboard)
			return EBTNodeResult::Failed;
		
		// Target location key
		Blackboard->SetValueAsVector(TargetLocationKey.SelectedKeyName, NearestAvailableFood->GetActorLocation());

		// Target food key
		Blackboard->SetValueAsObject(TargetFoodKey.SelectedKeyName, NearestAvailableFood);
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	else
	{
		return EBTNodeResult::Failed;
	}
}

void UBTTask_FindNearestAvailableFood::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
