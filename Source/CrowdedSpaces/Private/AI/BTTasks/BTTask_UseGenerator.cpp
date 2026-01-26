#include "AI/BTTasks/BTTask_UseGenerator.h"

#include "AI/NPCController.h"
#include "Build/BuildableGenerator.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Resources/EnergyComponent.h"

UBTTask_UseGenerator::UBTTask_UseGenerator(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Use Target Generator";

	bCreateNodeInstance = true; // Chaque NPC a sa propre instance    
	bNotifyTaskFinished = true;

	// Action
	NPCAction = ENPCAction::Work;
}

EBTNodeResult::Type UBTTask_UseGenerator::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	ABuildableGenerator* TargetGenerator = Cast<ABuildableGenerator>(Blackboard->GetValueAsObject(TargetGeneratorKey.SelectedKeyName));
	if (!TargetGenerator)
		return EBTNodeResult::Failed;
	
	// Set generator as working
	TargetGenerator->SetNPCWorking(true);
	
	return EBTNodeResult::InProgress; // Infinite as long as behavior tree doesn't change it (hungry, tired, event...)
}


void UBTTask_UseGenerator::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
	
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Use generator stop action");
	
	// Set generator as not working
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard)
		return;
	
	ABuildableGenerator* TargetGenerator = Cast<ABuildableGenerator>(Blackboard->GetValueAsObject(TargetGeneratorKey.SelectedKeyName));
	if (!TargetGenerator)
		return ;
	
	TargetGenerator->SetNPCWorking(false);
	
	StopAction();
}