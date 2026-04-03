#include "AI/BTTasks/BTTask_UseBuildableObject.h"

#include "AI/NPC.h"
#include "Build/BuildableObject.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Debug/CrowdedSpacesLogs.h"

UBTTask_UseBuildableObject::UBTTask_UseBuildableObject(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Use Buildable Object";

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

	bHasStartedUsing = false;

	return EBTNodeResult::InProgress;
}

void UBTTask_UseBuildableObject::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!NPC || !CurrentObject)
		return;

	if (!bHasStartedUsing)
	{
		CurrentObject->StartUsing(NPC);
		bHasStartedUsing = true;
	}

	if (!CurrentObject->CanBeUsed())
	{
		CS_LOG("Current used object set to cant be used");
		StopUsingClean();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	UResourceComponent* Resource = NPC->GetResourceComponentByType(ResourceTypeToCheck);
	if (!Resource)
		return;

	if (Resource->GetResource() >= Resource->GetMaxResource())
	{
		CS_LOG("Max resource eat");
		StopUsingClean();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
}

EBTNodeResult::Type UBTTask_UseBuildableObject::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CS_LOG("Abort use task");
	StopUsingClean();

	return EBTNodeResult::Aborted;
}

void UBTTask_UseBuildableObject::StopUsingClean()
{
	if (CurrentObject && NPC)
	{
		CurrentObject->StopUsing(NPC);
		CurrentObject->Release(NPC);
		NPC->SetCurrentObject(nullptr);
	}
}