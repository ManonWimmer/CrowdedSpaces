#include "AI/NPCController.h"

#include "AI/NPC.h"

ANPCController::ANPCController(FObjectInitializer const& ObjectInitializer)
{
}

void ANPCController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ANPC* const NPC = Cast<ANPC>(InPawn);
	if (!NPC)
		return;
	
	UBehaviorTree* const Tree = NPC->GetBehaviorTree();
	if (!Tree)
		return;
	
	UBlackboardComponent* b;
	UseBlackboard(Tree->BlackboardAsset,  b);
	Blackboard = b;
	RunBehaviorTree(Tree);
}
