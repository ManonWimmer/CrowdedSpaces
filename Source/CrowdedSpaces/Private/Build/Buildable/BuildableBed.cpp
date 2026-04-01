#include "Build/Buildable/BuildableBed.h"

#include "Build/BuildableRegistrySubsystem.h"

ABuildableBed::ABuildableBed()
{
	SelectionType = ESelectionType::Bed;
	ObjectType = EObjectType::Bed;
	NPCAction = ENPCActionWidget::Sleep;
}

void ABuildableBed::BeginPlay()
{
	Super::BeginPlay();

	if (!BRS)
		return;
	
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
	CurrentTask = UseObjectTask;
	
	if (!UsingNPC.IsValid())
		return false;
	
	UResourceComponent* EnergyComp = UsingNPC->GetResourceComponent<EResourceType::Energy>();
	if (!EnergyComp)
		return false;
	
	EnergyComp->SetIsInRegen(true);
	
	EnergyComp->OnResourceFull.AddDynamic(UseObjectTask, &UBTTask_UseBuildableObject::OnStopAction);

	UResourceComponent* FoodComp = UsingNPC->GetResourceComponent<EResourceType::Food>();
	if (!FoodComp)
		return false;
	
	FoodComp->ToggleResourceTimer(); 
	
	return true;
}

bool ABuildableBed::StopUsingImplementation(UBTTask_UseBuildableObject* UseObjectTask)
{
	CurrentTask = nullptr;
	
	if (!UsingNPC.IsValid())
		return false;
	
	UResourceComponent* EnergyComp = UsingNPC->GetResourceComponent<EResourceType::Energy>();
	if (!EnergyComp)
		return false;

	EnergyComp->SetIsInRegen(false);

	EnergyComp->OnResourceFull.RemoveDynamic(UseObjectTask, &UBTTask_UseBuildableObject::OnStopAction);

	UResourceComponent* FoodComp = UsingNPC->GetResourceComponent<EResourceType::Food>();
	if (!FoodComp)
		return false;
	
	FoodComp->ToggleResourceTimer(); 
	
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

