#include "Action/Actions/Action_NPC_StopAction.h"

#include "AIController.h"
#include "AI/NPC.h"
#include "BehaviorTree/BlackboardComponent.h"

UAction_NPC_StopAction::UAction_NPC_StopAction()
{
	ActionName = "Stop Action";
}

bool UAction_NPC_StopAction::CanExecute_Implementation(AActor* Instigator) const
{
	// Check if npc has generator or train station set in blackboard
	
	const ANPC* SelectedNPC = ActionSubsystem->GetSelectedNPC();
	if (!SelectedNPC)
		return false;
	
	AAIController* AIController = Cast<AAIController>(SelectedNPC->GetController());
	if (!AIController)
		return false;
	UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
	if (!Blackboard)
		return false;
	
	if (Blackboard->GetValueAsObject("Generator") == nullptr && Blackboard->GetValueAsObject("TrainingStation") == nullptr)
		return false;

	return true;
}

void UAction_NPC_StopAction::Execute_Implementation(AActor* Instigator)
{
	Super::Execute_Implementation(Instigator);

	// Reset generator & training values in blackboard
	
	const ANPC* SelectedNPC = ActionSubsystem->GetSelectedNPC();
	if (!SelectedNPC)
		return;
	
	AAIController* AIController = Cast<AAIController>(SelectedNPC->GetController());
	if (!AIController)
		return;

	UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
	if (!Blackboard)
		return;

	Blackboard->SetValueAsObject("Generator", nullptr);
	Blackboard->SetValueAsObject("GeneratorSlot", nullptr);
	Blackboard->SetValueAsVector("GeneratorLocation", FVector::Zero());
			
	Blackboard->SetValueAsObject("TrainingStation", nullptr);
	Blackboard->SetValueAsObject("TrainingStationSlot", nullptr);
	Blackboard->SetValueAsVector("TrainingStationLocation", FVector::Zero());
}
