#include "AI/BTTasks/BTTask_FindNearestAvailableBed.h"

#include "EngineUtils.h"
#include "AI/NPCController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Build/Buildable/BuildableBed.h"
#include "Build/BuildableRegistrySubsystem.h"

UBTTask_FindNearestAvailableBed::UBTTask_FindNearestAvailableBed(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find Nearest Available Bed Location In NavMesh";
	
	bCreateNodeInstance = true; // Chaque NPC a sa propre instance    
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_FindNearestAvailableBed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	ABuildableBed* NearestAvailableBed = nullptr;
	float NearestDistance = 0.0f;

	UBuildableRegistrySubsystem* BRS = World->GetSubsystem<UBuildableRegistrySubsystem>();
	if (!BRS)
		return EBTNodeResult::Failed;
	
	for (TWeakObjectPtr<ABuildableBed> Bed : BRS->Beds) 
	{
		if (!Bed.IsValid())
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
					NearestAvailableBed = Bed.Get();
					NearestDistance = Distance;
				}
			}
			else
			{
				NearestAvailableBed = Bed.Get();
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

void UBTTask_FindNearestAvailableBed::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
