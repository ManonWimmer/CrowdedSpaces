#include "AI/NPCController.h"

#include "AI/NPC.h"

ANPCController::ANPCController(FObjectInitializer const& ObjectInitializer)
{
}

void ANPCController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (ANPC* const NPC = Cast<ANPC>(InPawn))
	{
		if (UBehaviorTree* const Tree = NPC->GetBehaviorTree())
		{
			UBlackboardComponent* b;
			UseBlackboard(Tree->BlackboardAsset,  b);
			Blackboard = b;
			RunBehaviorTree(Tree);
		}
	}
}
