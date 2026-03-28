#include "AI/BTTasks/BTTask_FindNearestAvailableBuildableObject.h"

#include "AI/NPCController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Build/BuildableRegistrySubsystem.h"

UBTTask_FindNearestAvailableBuildableObject::UBTTask_FindNearestAvailableBuildableObject(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find Nearest Available Buildable Object Location In NavMesh";
	
	bCreateNodeInstance = true;  
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_FindNearestAvailableBuildableObject::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	if (BuildableObjectType == EObjectType::Default)
		return EBTNodeResult::Failed;
	
	TObjectPtr<ANPCController> const Controller = Cast<ANPCController>(OwnerComp.GetAIOwner());
	if (!Controller)
		return EBTNodeResult::Failed;

	NPC = Cast<ANPC>(Controller->GetPawn());
	if (!NPC)
		return EBTNodeResult::Failed;

	TObjectPtr<UWorld> World = NPC->GetWorld();
	if (!World)
		return EBTNodeResult::Failed;
	
	FVector const Origin = NPC->GetActorLocation();
	ABuildableObject* NearestAvailableObject = nullptr;
	float NearestDistance = FLT_MAX;

	TObjectPtr<UBuildableRegistrySubsystem> BRS = World->GetSubsystem<UBuildableRegistrySubsystem>();
	if (!BRS)
		return EBTNodeResult::Failed;

	UE_LOG(LogTemp, Warning, TEXT("\n===== SEARCH BUILDABLE ====="));
	UE_LOG(LogTemp, Warning, TEXT("NPC: %s | Type: %d | Origin: %s"),
		*GetNameSafe(NPC),
		(int32)BuildableObjectType,
		*Origin.ToString());

	UE_LOG(LogTemp, Warning, TEXT("SearchRadius: %.1f"), SearchRadius);
	UE_LOG(LogTemp, Warning, TEXT("Candidates total: %d"), BRS->BuildableObjects.Num());

	// Get nearest available buildable object
	for (TWeakObjectPtr<ABuildableObject> Object : BRS->BuildableObjects) 
	{
		if (!Object.IsValid())
			continue; 
		
		if (Object->GetObjectType() != BuildableObjectType)
			continue;
		
		if (Object->HasNPCUsing())
			continue;

		if (!Object->CanBeUsed())
		{
			UE_LOG(LogTemp, Warning, TEXT("CanBeUsed = false"));
			continue;
		}

		if (Object->IsReservedByOther(NPC))
		{
			UE_LOG(LogTemp, Warning, TEXT("Reserved by other"));
			continue;
		}

		// Check same generator type as npc should work on
		if (BuildableObjectType == EObjectType::Generator)
		{
			ABuildableGenerator* Generator = Cast<ABuildableGenerator>(Object);
			if (NPC->GetWorkOnGeneratorType() != Generator->GetProductionType())
				continue;
		}
		
		float Distance = FVector::Distance(Origin, Object->GetActorLocation());
		
		if (Distance > SearchRadius)
		{
			UE_LOG(LogTemp, Warning, TEXT("Out of range"));
			continue;
		}

		UE_LOG(LogTemp, Warning, TEXT("Valid candidate"));

		if (!NearestAvailableObject || Distance < NearestDistance)
		{
			UE_LOG(LogTemp, Warning, TEXT(">>> NEW NEAREST: %s (%.1f)"),
			  *Object->GetName(),
			  Distance);
			
			NearestAvailableObject = Object.Get();
			NearestDistance = Distance;
		}
	}

	// Success or Failed + set keys
	if (NearestAvailableObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("\n===== RESULT ====="));
        UE_LOG(LogTemp, Warning, TEXT("Selected: %s"), *NearestAvailableObject->GetName());
		
		TObjectPtr<UBlackboardComponent> Blackboard = OwnerComp.GetBlackboardComponent();
		if (!Blackboard)
			return EBTNodeResult::Failed;

		if (!NearestAvailableObject->TryReserve(NPC))
			return EBTNodeResult::Failed;

		// Use interaction slot for npc target location
		FInteractionSlot* Slot = NearestAvailableObject->ReserveSlot(NPC);
		if (!Slot)
			return EBTNodeResult::Failed;

		Blackboard->SetValueAsVector(TargetLocationKey.SelectedKeyName, Slot->Point->GetComponentLocation());
		Blackboard->SetValueAsObject(TargetObjectKey.SelectedKeyName, NearestAvailableObject);
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("NO VALID OBJECT FOUND"));
		return EBTNodeResult::Failed;
	}
}

void UBTTask_FindNearestAvailableBuildableObject::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
