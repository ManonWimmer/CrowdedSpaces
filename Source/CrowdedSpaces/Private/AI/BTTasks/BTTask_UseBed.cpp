#include "AI/BTTasks/BTTask_UseBed.h"

#include "AI/NPCController.h"
#include "Build/BuildableBed.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Resources/EnergyComponent.h"

UBTTask_UseBed::UBTTask_UseBed(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Use Target Bed";

	bCreateNodeInstance = true; // Chaque NPC a sa propre instance    
	bNotifyTaskFinished = true;

	// Action
	NPCAction = ENPCAction::Sleep;
}

EBTNodeResult::Type UBTTask_UseBed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	ABuildableBed* TargetBed = Cast<ABuildableBed>(Blackboard->GetValueAsObject(TargetBedKey.SelectedKeyName));
	if (!TargetBed)
		return EBTNodeResult::Failed;
	
	// Set bed as unavailable
	TargetBed->SetAvailable(false);

	// Get NPC EnergyComponent
	EnergyComp = NPC->FindComponentByClass<UEnergyComponent>();
	if (!EnergyComp.IsValid())
		return EBTNodeResult::Failed;

	// Set NPC sleeping
	EnergyComp->SetSleeping(true);

	// Bind callback to energy full
	EnergyComp->OnEnergyFull.AddDynamic(this, &UBTTask_UseBed::OnEnergyFull);

	OwnerCompPtr = &OwnerComp;
	
	return EBTNodeResult::InProgress;
}

void UBTTask_UseBed::OnEnergyFull()
{
	if (!OwnerCompPtr.IsValid() || !EnergyComp.IsValid())
		return;

	ANPCController* Controller = Cast<ANPCController>(OwnerCompPtr->GetAIOwner());
	if (!Controller)
		return;

	NPC = Cast<ANPC>(Controller->GetPawn());
	if (!NPC)
		return;

	// Unbind
	EnergyComp->OnEnergyFull.RemoveDynamic(this, &UBTTask_UseBed::OnEnergyFull);

	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "On energy full");

	FinishLatentTask(*OwnerCompPtr, EBTNodeResult::Succeeded);
}

void UBTTask_UseBed::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
	
	// Set bed as available
	UBlackboardComponent* Blackboard = OwnerCompPtr->GetBlackboardComponent();
	if (!Blackboard)
		return;
	
	ABuildableBed* TargetBed = Cast<ABuildableBed>(Blackboard->GetValueAsObject(TargetBedKey.SelectedKeyName));
	if (!TargetBed)
		return;
	
	TargetBed->SetAvailable(true); 

	// Set NPC not sleeping
	EnergyComp->SetSleeping(false);
	
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Use Target Bed stop action");
	
	StopAction();
}