#include "AI/BTTask_CheckNeedsToEat.h"
#include "AI/NPC.h"
#include "AI/NPCController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_CheckNeedsToEat::UBTTask_CheckNeedsToEat(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Check Needs To Eat";
}

EBTNodeResult::Type UBTTask_CheckNeedsToEat::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ANPCController* const Controller = Cast<ANPCController>(OwnerComp.GetAIOwner());
	if (!Controller)
		return EBTNodeResult::Failed;

	ANPC* const NPC = Cast<ANPC>(Controller->GetPawn());
	if (!NPC)
		return EBTNodeResult::Failed;
	
	UFoodComponent* FoodComponent = NPC->GetFoodComponent();
	if (!FoodComponent)
		return EBTNodeResult::Failed;

	if (FoodComponent->GetFood() < EatMinimum)
	{
		// Needs to eat - Success
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	else
	{
		// Doesn't need to eat - Failed
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Failed;
}
