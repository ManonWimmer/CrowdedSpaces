#include "Action/Actions/Action_TrainingStation_Train.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Build/SlotComponent.h"
#include "Build/Buildable/BuildableTrainingStation.h"

UAction_TrainingStation_Train::UAction_TrainingStation_Train()
{
	ActionName = "Train";
}

bool UAction_TrainingStation_Train::CanExecute_Implementation(AActor* Instigator) const
{
	const ABuildableTrainingStation* TrainingStation = Cast<ABuildableTrainingStation>(Instigator);
	if (!TrainingStation)
		return false;

	const ANPC* SelectedNPC = ActionSubsystem->GetSelectedNPC();
	if (!SelectedNPC)
		return false;

	if (!TrainingStation->CanBeUsed())
		return false;

	return TrainingStation->IsAvailableForReservation(SelectedNPC);
}

void UAction_TrainingStation_Train::Execute_Implementation(AActor* Instigator)
{
	Super::Execute_Implementation(Instigator);

	// Set training values in selected npc blackboard
	
	ABuildableTrainingStation* TrainingStation = Cast<ABuildableTrainingStation>(Instigator);
	if (!TrainingStation)
		return;
	
	if (!ActionSubsystem)
		return;

	const ANPC* SelectedNPC = ActionSubsystem->GetSelectedNPC();
	if (!SelectedNPC)
		return;

	USlotComponent* Slot = TrainingStation->GetNearestFreeSlot(SelectedNPC->GetActorLocation());
	if (!Slot)
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
	
	Blackboard->SetValueAsObject("TrainingStation", TrainingStation);
	Blackboard->SetValueAsObject("TrainingStationSlot", Slot);
	Blackboard->SetValueAsVector("TrainingStationLocation", Slot->GetComponentLocation());
}
