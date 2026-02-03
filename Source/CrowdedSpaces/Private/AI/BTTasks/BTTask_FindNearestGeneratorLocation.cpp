#include "AI/BTTasks/BTTask_FindNearestGeneratorLocation.h"

#include "EngineUtils.h"
#include "AI/NPCController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Build/BuildableRegistrySubsystem.h"
#include "Build/Buildable/BuildableGenerator.h"

UBTTask_FindNearestGeneratorLocation::UBTTask_FindNearestGeneratorLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find Nearest Generator Location In NavMesh";

	bCreateNodeInstance = true; // Chaque NPC a sa propre instance    
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_FindNearestGeneratorLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TObjectPtr<ANPCController>const Controller = Cast<ANPCController>(OwnerComp.GetAIOwner());
	if (!Controller)
		return EBTNodeResult::Failed;

	NPC = Cast<ANPC>(Controller->GetPawn());
	if (!NPC)
		return EBTNodeResult::Failed;
	
	TObjectPtr<UWorld> World = NPC->GetWorld();
	if (!World)
		return EBTNodeResult::Failed;

	FVector const Origin = NPC->GetActorLocation();
	ABuildableGenerator* NearestGenerator = nullptr;
	float NearestDistance = 0.0f;
	
	TObjectPtr<UBuildableRegistrySubsystem> BRS = World->GetSubsystem<UBuildableRegistrySubsystem>();
	if (!BRS)
		return EBTNodeResult::Failed;
	
	for (TWeakObjectPtr<ABuildableGenerator> Generator : BRS->Generators) 
	{
		if (!Generator.IsValid())
			continue;
		
		// Get nearest generator of production type in radius
		float Distance = FVector::Distance(Origin, Generator->GetActorLocation());
		if (Distance < SearchRadius / 2)
		{
			if (NearestGenerator)
			{
				if (Distance < NearestDistance)
				{
					NearestGenerator = Generator.Get();
					NearestDistance = Distance;
				}
			}
			else
			{
				NearestGenerator = Generator.Get();
				NearestDistance = Distance;
			}
		}
	}

	// Success or Failed
	if (NearestGenerator)
	{
		TObjectPtr<UBlackboardComponent> Blackboard = OwnerComp.GetBlackboardComponent();
		if (!Blackboard)
			return EBTNodeResult::Failed;

		// Target location key
		Blackboard->SetValueAsVector(TargetLocationKey.SelectedKeyName, NearestGenerator->GetActorLocation());
		
		// Target generator key
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
