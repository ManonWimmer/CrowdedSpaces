#include "Action/Actions/Action_TrainingStation_Train.h"

#include "Object/Buildable/BuildableTrainingStation.h"

UAction_TrainingStation_Train::UAction_TrainingStation_Train()
{
	ActionName = "Train";
}

bool UAction_TrainingStation_Train::CanExecute_Implementation(AActor* Instigator) const
{
	const TObjectPtr<ABuildableTrainingStation> TrainingStation = Cast<ABuildableTrainingStation>(Instigator);
	if (!TrainingStation)
		return false;

	const TObjectPtr<ANPC> SelectedNPC = ActionSubsystem->GetSelectedNPC();
	if (!SelectedNPC)
		return false;

	if (!TrainingStation->CanBeUsed())
		return false;

	return TrainingStation->IsAvailableForReservation(SelectedNPC);
}

void UAction_TrainingStation_Train::Execute_Implementation(AActor* Instigator)
{
	Super::Execute_Implementation(Instigator);

	const TObjectPtr<ABuildableTrainingStation> TrainingStation = Cast<ABuildableTrainingStation>(Instigator);
	if (!TrainingStation)
		return;
	
	if (!ActionSubsystem)
		return;

	const TObjectPtr<ANPC> SelectedNPC = ActionSubsystem->GetSelectedNPC();
	if (!SelectedNPC)
		return;
	
	SelectedNPC->SetActionObject(TrainingStation);
}
