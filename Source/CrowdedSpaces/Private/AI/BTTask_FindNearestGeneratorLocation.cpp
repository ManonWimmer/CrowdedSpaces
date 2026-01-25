#include "AI/BTTask_FindNearestGeneratorLocation.h"
#include "AI/NPCController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Build/BuildableGenerator.h"
#include "Customizations/MathStructProxyCustomizations.h"

UBTTask_FindNearestGeneratorLocation::UBTTask_FindNearestGeneratorLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find Nearest Generator Location In NavMesh";
}

EBTNodeResult::Type UBTTask_FindNearestGeneratorLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ANPCController* const Controller = Cast<ANPCController>(OwnerComp.GetAIOwner());
	if (!Controller)
		return EBTNodeResult::Failed;

	APawn* const NPC = Controller->GetPawn();
	if (!NPC)
		return EBTNodeResult::Failed;

	FVector const Origin = NPC->GetActorLocation();
	ABuildableGenerator* NearestGenerator = nullptr;
	float NearestDistance = 0.0f;
	
	for (TObjectIterator<ABuildableGenerator> Itr; Itr; ++Itr)
	{
		ABuildableGenerator* Generator = *Itr;
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
		Blackboard->SetValueAsVector(GetSelectedBlackboardKey(), NearestGenerator->GetActorLocation());
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	else
	{
		return EBTNodeResult::Failed;
	}
}
