#include "Build/Buildable/BuildableBed.h"

#include "Build/BuildableRegistrySubsystem.h"

ABuildableBed::ABuildableBed()
{
	SelectionType = ESelectionType::Bed;
	ObjectType = EObjectType::Bed;
	NPCAction = ENPCAction::Sleep;
}

void ABuildableBed::BeginPlay()
{
	Super::BeginPlay();

	if (!BRS)
		return;

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "BeginPlay avec BRS bed");
	
	BRS->RegisterBed(this);
}

void ABuildableBed::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (!BRS)
		return;

	BRS->UnregisterBed(this);
}

bool ABuildableBed::StartUsingImplementation(UBTTask_UseBuildableObject* UseObjectTask)
{
	CurrentTasks.Add(UseObjectTask);
	
	UEnergyComponent* EnergyComp = UsingNPC->FindComponentByClass<UEnergyComponent>();
	if (!EnergyComp)
		return false;
	
	EnergyComp->SetSleeping(true);
	
	EnergyComp->OnEnergyFull.AddDynamic(UseObjectTask, &UBTTask_UseBuildableObject::OnStopAction);

	return true;
}

bool ABuildableBed::StopUsingImplementation(UBTTask_UseBuildableObject* UseObjectTask)
{
	CurrentTasks.Remove(UseObjectTask);
	
	UEnergyComponent* EnergyComp = UsingNPC->FindComponentByClass<UEnergyComponent>();
	if (!EnergyComp)
		return false;

	EnergyComp->SetSleeping(false);

	EnergyComp->OnEnergyFull.RemoveDynamic(UseObjectTask, &UBTTask_UseBuildableObject::OnStopAction);
	
	return true;
}

#pragma region Selectable
void ABuildableBed::OnSelected()
{
}

void ABuildableBed::OnDeselected()
{
}
#pragma endregion

