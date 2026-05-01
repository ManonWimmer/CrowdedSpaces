#include "Action/Actions/Action_TrainingStation_Train.h"

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
	
	ABuildableTrainingStation* TrainingStation = Cast<ABuildableTrainingStation>(Instigator);
	if (!TrainingStation)
		return;
	
	if (!ActionSubsystem)
		return;

	ANPC* SelectedNPC = ActionSubsystem->GetSelectedNPC();
	if (!SelectedNPC)
		return;
	
	SelectedNPC->SetTrainingStation(TrainingStation);
}
