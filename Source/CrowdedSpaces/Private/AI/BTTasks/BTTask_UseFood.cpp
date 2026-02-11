#include "AI/BTTasks/BTTask_UseFood.h"

#include "AI/NPCController.h"
#include "Build/Buildable/BuildableFood.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Resources/FoodComponent.h"

UBTTask_UseFood::UBTTask_UseFood(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Use Target Food";

	bCreateNodeInstance = true; // Chaque NPC a sa propre instance    
	bNotifyTaskFinished = true;

	// Action
	NPCAction = ENPCAction::Eat;
}

EBTNodeResult::Type UBTTask_UseFood::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TObjectPtr<ANPCController> Controller = Cast<ANPCController>(OwnerComp.GetAIOwner());
	if (!Controller)
		return EBTNodeResult::Failed;

	NPC = Cast<ANPC>(Controller->GetPawn());
	if (!NPC)
		return EBTNodeResult::Failed;
	
	// Get the target bed from blackboard
	TObjectPtr<UBlackboardComponent> Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard)
		return EBTNodeResult::Failed;

	TObjectPtr<ABuildableFood> TargetFood = Cast<ABuildableFood>(Blackboard->GetValueAsObject(TargetFoodKey.SelectedKeyName));
	if (!TargetFood)
		return EBTNodeResult::Failed;


	/*
	if (TargetFood->HasNPCEating())
		return EBTNodeResult::Failed;

	if (TargetFood->IsReservedByOther(NPC))
		return EBTNodeResult::Failed;
	
	TargetFood->StartEating(NPC);
	*/
	
	StartAction();
	
	FoodComp = NPC->FindComponentByClass<UFoodComponent>();
	if (!FoodComp.IsValid())
		return EBTNodeResult::Failed;
	
	FoodComp->SetEating(true);
	
	FoodComp->OnFoodFull.AddDynamic(this, &UBTTask_UseFood::OnFoodFull);

	OwnerCompPtr = &OwnerComp;
	
	return EBTNodeResult::InProgress;
}

void UBTTask_UseFood::OnFoodFull()
{
	if (!OwnerCompPtr.IsValid() || !FoodComp.IsValid())
		return;

	TObjectPtr<ANPCController> Controller = Cast<ANPCController>(OwnerCompPtr->GetAIOwner());
	if (!Controller)
		return;

	NPC = Cast<ANPC>(Controller->GetPawn());
	if (!NPC)
		return;
	
	FoodComp->OnFoodFull.RemoveDynamic(this, &UBTTask_UseFood::OnFoodFull);

	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "On food full");

	FinishLatentTask(*OwnerCompPtr, EBTNodeResult::Succeeded);
}

void UBTTask_UseFood::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
	
	TObjectPtr<UBlackboardComponent> Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard)
		return;
	
	TObjectPtr<ABuildableFood> TargetFood = Cast<ABuildableFood>(Blackboard->GetValueAsObject(TargetFoodKey.SelectedKeyName));
	if (!TargetFood)
		return;
	
	//TargetFood->StopEating(NPC);
	
	if (FoodComp.IsValid())
		FoodComp->SetEating(false);
	
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Use target food stop action");
	
	StopAction();
}