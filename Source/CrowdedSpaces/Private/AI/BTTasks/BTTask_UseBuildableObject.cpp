#include "AI/BTTasks/BTTask_UseBuildableObject.h"

#include "AI/NPC.h"
#include "Object/UsableObject.h"
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
	const AAIController* AI = OwnerComp.GetAIOwner();
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
	AUsableObject* NewObject = NPC->GetCurrentObject();

	if (NewObject != CurrentObject)
	{
		StopUsing();
		FinishLatentTask(OwnerComp, EBTNodeResult::Aborted);
		return;
	}
	
	if (!NPC || !CurrentObject || !IsValid(CurrentObject) || CurrentObject->bIsBeingDestroyed)
	{
		StopUsing();
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
	
	if (!NPC || !CurrentObject)
		return;

	if (!bHasStartedUsing)
	{
		CurrentObject->StartUsing(NPC);
		bHasStartedUsing = true;
	}

	if (!CurrentObject->CanBeUsed())
	{
		CS_LOG_WARNING("Current used object set to cant be used, stop using");
		
		StopUsing();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		
		return;
	}

	if (CurrentObject->GetUsingResourceTypeToCheck() == EResourceType::None)
		return;
	
	const UResourceComponent* Resource = NPC->GetResourceComponentByType(CurrentObject->GetUsingResourceTypeToCheck());
	if (!Resource)
		return;

	if (Resource->GetResource() >= Resource->GetMaxResource())
	{
		CS_LOG_WARNING("Using at max resource, stop using");
		
		StopUsing();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		
		return;
	}
}

EBTNodeResult::Type UBTTask_UseBuildableObject::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CS_LOG_WARNING("Abort use task");
	StopUsing();

	return EBTNodeResult::Aborted;
}

void UBTTask_UseBuildableObject::StopUsing() const
{
	if (!CurrentObject || !NPC)
		return;

	CS_LOG_WARNING("Stop using object");
	
	CurrentObject->StopUsing(NPC);
	CurrentObject->Release(NPC);

	if (NPC->GetActionObject() == CurrentObject)
		NPC->StopAction();
}
