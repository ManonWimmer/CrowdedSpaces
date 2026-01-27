#include "AI/BTTasks/BTTask_FindNearestGeneratorLocation.h"

#include "EngineUtils.h"
#include "AI/NPCController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Build/Buildable/BuildableGenerator.h"

UBTTask_FindNearestGeneratorLocation::UBTTask_FindNearestGeneratorLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find Nearest Generator Location In NavMesh";

	bCreateNodeInstance = true; // Chaque NPC a sa propre instance    
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_FindNearestGeneratorLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	ABuildableGenerator* NearestGenerator = nullptr;
	float NearestDistance = 0.0f;
	
	for (TActorIterator<ABuildableGenerator> It(World); It; ++It)
	{
		ABuildableGenerator* Generator = *It;
		if (!Generator)
			continue;
		
		// Get nearest generator of production type in radius
		if (Generator->GetProductionComponent()->ProductionType != ProductionType)
			continue;
		
		float Distance = FVector::Distance(Origin, Generator->GetActorLocation());
		if (Distance < SearchRadius / 2)
		{
			if (NearestGenerator)
			{
				if (Distance < NearestDistance)
				{
					NearestGenerator = Generator;
					NearestDistance = Distance;
				}
			}
			else
			{
				NearestGenerator = Generator;
				NearestDistance = Distance;
			}
		}
	}

	// Success or Failed
	if (NearestGenerator)
	{
		UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
		if (!Blackboard)
			return EBTNodeResult::Failed;

		// Target location key
		Blackboard->SetValueAsVector(TargetLocationKey.SelectedKeyName, NearestGenerator->GetActorLocation());

		// Target bed key
		Blackboard->SetValueAsObject(TargetGeneratorKey.SelectedKeyName, NearestGenerator);
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	else
	{
		return EBTNodeResult::Failed;
	}
}

void UBTTask_FindNearestGeneratorLocation::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
