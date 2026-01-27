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
	ANPCController* Controller = Cast<ANPCController>(OwnerComp.GetAIOwner());
	if (!Controller)
		return EBTNodeResult::Failed;

	NPC = Cast<ANPC>(Controller->GetPawn());
	if (!NPC)
		return EBTNodeResult::Failed;

	// Action
	StartAction();
	
	// Get the target bed from blackboard
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard)
		return EBTNodeResult::Failed;

	ABuildableFood* TargetFood = Cast<ABuildableFood>(Blackboard->GetValueAsObject(TargetFoodKey.SelectedKeyName));
	if (!TargetFood)
		return EBTNodeResult::Failed;
	
	// Set food as unavailable
	TargetFood->SetAvailable(false);

	// Get NPC FoodComponent
	FoodComp = NPC->FindComponentByClass<UFoodComponent>();
	if (!FoodComp.IsValid())
		return EBTNodeResult::Failed;

	// Set NPC eating
	FoodComp->SetEating(true);

	// Bind callback to food full
	FoodComp->OnFoodFull.AddDynamic(this, &UBTTask_UseFood::OnFoodFull);

	OwnerCompPtr = &OwnerComp;
	
	return EBTNodeResult::InProgress;
}

void UBTTask_UseFood::OnFoodFull()
{
	if (!OwnerCompPtr.IsValid() || !FoodComp.IsValid())
		return;

	ANPCController* Controller = Cast<ANPCController>(OwnerCompPtr->GetAIOwner());
	if (!Controller)
		return;

	NPC = Cast<ANPC>(Controller->GetPawn());
	if (!NPC)
		return;

	// Unbind
	FoodComp->OnFoodFull.RemoveDynamic(this, &UBTTask_UseFood::OnFoodFull);

	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "On food full");

	FinishLatentTask(*OwnerCompPtr, EBTNodeResult::Succeeded);
}

void UBTTask_UseFood::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
	
	// Set food as available
	UBlackboardComponent* Blackboard = OwnerCompPtr->GetBlackboardComponent();
	if (!Blackboard)
		return;
	
	ABuildableFood* TargetFood = Cast<ABuildableFood>(Blackboard->GetValueAsObject(TargetFoodKey.SelectedKeyName));
	if (!TargetFood)
		return;
	
	TargetFood->SetAvailable(true); 

	// Set NPC not eating
	FoodComp->SetEating(false);
	
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Use target food stop action");
	
	StopAction();
}