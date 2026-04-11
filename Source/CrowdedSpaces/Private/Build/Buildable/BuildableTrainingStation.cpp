#include "Build/Buildable/BuildableTrainingStation.h"

#include "Build/BuildableRegistrySubsystem.h"

ABuildableTrainingStation::ABuildableTrainingStation()
{
	SelectionType = ESelectionType::TrainingStation;
	ObjectType = EObjectType::TrainingStation;
	NPCAction = ENPCActionType::Train;
}

void ABuildableTrainingStation::BeginPlay()
{
	Super::BeginPlay();
	
	if (!BRS)
		return;

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Register training station");

	BRS->RegisterTrainingStation(this);
}

void ABuildableTrainingStation::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (!BRS)
		return;
	
	BRS->UnregisterTrainingStation(this);
}

bool ABuildableTrainingStation::StartUsingImplementation(ANPC* NPC)
{
	// train skill type add exp

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, "Start training");
	
	return true;
}

bool ABuildableTrainingStation::StopUsingImplementation(ANPC* NPC)
{
	// stop train skill
	
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, "Stop training");
	
	return true;
}

#pragma region Selectable
void ABuildableTrainingStation::OnSelected()
{
}

void ABuildableTrainingStation::OnDeselected()
{
}
#pragma endregion


