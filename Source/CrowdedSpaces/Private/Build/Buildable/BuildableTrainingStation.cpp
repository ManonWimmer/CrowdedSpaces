#include "Build/Buildable/BuildableTrainingStation.h"

#include "Build/BuildableRegistrySubsystem.h"
#include "Training/TrainingSubsystem.h"

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

	const UWorld* World = GetWorld();
	if (!World)
		return;
	
	TrainingSubsystem = World->GetSubsystem<UTrainingSubsystem>();
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
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, "Start training");

	TrainingSubsystem->AddTrainingNPC(NPC);
	
	return true;
}

bool ABuildableTrainingStation::StopUsingImplementation(ANPC* NPC)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, "Stop training");
	
	TrainingSubsystem->RemoveTrainingNPC(NPC);
	
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


