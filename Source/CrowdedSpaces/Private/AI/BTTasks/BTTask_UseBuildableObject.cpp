#include "AI/BTTasks/BTTask_UseBuildableObject.h"
#include "AI/NPC.h"
#include "Build/BuildableObject.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTTask_UseBuildableObject::UBTTask_UseBuildableObject()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_UseBuildableObject::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AI = OwnerComp.GetAIOwner();
	if (!AI)
		return EBTNodeResult::Failed;

	NPC = Cast<ANPC>(AI->GetPawn());
	if (!NPC)
		return EBTNodeResult::Failed;

	CurrentObject = NPC->GetCurrentObject();
	if (!CurrentObject)
		return EBTNodeResult::Failed;

	// reset state (comme EnterState Sleep/Eat)
	bHasStartedUsing = false;

	return EBTNodeResult::InProgress;
}

void UBTTask_UseBuildableObject::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!NPC || !CurrentObject)
		return;

	// 1. START ONCE (équivalent bIsUsingObject)
	if (!bHasStartedUsing)
	{
		CurrentObject->StartUsing(NPC);
		bHasStartedUsing = true;
	}

	// 2. GET RESOURCE (comme ta state machine)
	UResourceComponent* Resource = NPC->GetResourceComponentByType(ResourceTypeToCheck);

	if (!Resource)
		return;

	// 3. CONDITION DE FIN (identique à Sleep/Eat)
	if (Resource->GetResource() >= 100.f)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

	if (!CurrentObject->CanBeUsed())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
}

EBTNodeResult::Type UBTTask_UseBuildableObject::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	StopUsingSafe();
	return EBTNodeResult::Aborted;
}

void UBTTask_UseBuildableObject::StopUsingSafe()
{
	if (!NPC || !CurrentObject)
		return;

	CurrentObject->StopUsing(NPC);

	bHasStartedUsing = false;
}