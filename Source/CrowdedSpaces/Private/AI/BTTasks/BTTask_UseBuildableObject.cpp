#include "AI/BTTasks/BTTask_UseBuildableObject.h"

#include "AI/NPC.h"
#include "Build/BuildableObject.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Debug/CrowdedSpacesLogs.h"

UBTTask_UseBuildableObject::UBTTask_UseBuildableObject(FObjectInitializer const& ObjectInitializer):
	ResourceTypeToCheck()
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
	ABuildableObject* ActionObject = Cast<ABuildableObject>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("ActionObject"));
	if (ActionObject && CurrentObject != ActionObject)
	{
		UE_LOG(LogTemp, Error, TEXT("Return action object != current object use"));
		StopUsing();
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
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

	if (ResourceTypeToCheck == EResourceType::None)
		return;
	
	const UResourceComponent* Resource = NPC->GetResourceComponentByType(ResourceTypeToCheck);
	if (!Resource)
		return;

	if (Resource->GetResource() >= Resource->GetMaxResource())
	{
		CS_LOG_WARNING("Max resource, stop using");
		
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
