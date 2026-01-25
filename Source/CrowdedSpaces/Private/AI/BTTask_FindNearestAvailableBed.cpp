#include "AI/BTTask_FindNearestAvailableBed.h"

#include "EngineUtils.h"
#include "AI/NPCController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Build/BuildableBed.h"

UBTTask_FindNearestAvailableBed::UBTTask_FindNearestAvailableBed(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find Nearest Available Bed Location In NavMesh";
}

EBTNodeResult::Type UBTTask_FindNearestAvailableBed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ANPCController* const Controller = Cast<ANPCController>(OwnerComp.GetAIOwner());
	if (!Controller)
		return EBTNodeResult::Failed;

	APawn* const NPC = Controller->GetPawn();
	if (!NPC)
		return EBTNodeResult::Failed;

	UWorld* World = NPC->GetWorld();
	if (!World)
		return EBTNodeResult::Failed;

	FVector const Origin = NPC->GetActorLocation();
	ABuildableBed* NearestAvailableBed = nullptr;
	float NearestDistance = 0.0f;
	
	for (TActorIterator<ABuildableBed> It(World); It; ++It) // TObjectIterator marchait pas en renvoyait tjrs en actor location 0,0,-78
	{
		ABuildableBed* Bed = *It;
		if (!Bed || !IsValid(Bed))
			continue; 
		
		// Get nearest bed AVAILABLE
		if (!Bed->IsAvailable())
			continue;
		
		float Distance = FVector::Distance(Origin, Bed->GetActorLocation());
		if (Distance < SearchRadius / 2)
		{
			if (NearestAvailableBed)
			{
				if (Distance < NearestDistance)
				{
					NearestAvailableBed = Bed;
					NearestDistance = Distance;
				}
			}
			else
			{
				NearestAvailableBed = Bed;
				NearestDistance = Distance;
			}
		}
	}

	// Success or Failed + set keys
	if (NearestAvailableBed)
	{
		UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
		if (!Blackboard)
			return EBTNodeResult::Failed;
		
		// Target location key
		Blackboard->SetValueAsVector(TargetLocationKey.SelectedKeyName, NearestAvailableBed->GetActorLocation());

		// Target bed key
		Blackboard->SetValueAsObject(TargetBedKey.SelectedKeyName, NearestAvailableBed);
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	else
	{
		return EBTNodeResult::Failed;
	}
}
