#include "Build/Buildable/BuildableBed.h"

#include "Build/BuildableRegistrySubsystem.h"

ABuildableBed::ABuildableBed()
{
	SelectionType = ESelectionType::Bed;
	ObjectType = EObjectType::Bed;
	NPCUseAction = ENPCActionType::Sleep;
	NPCMoveAction = ENPCActionType::MoveToBed;
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

#pragma region Use Object
bool ABuildableBed::StartUsingImplementation(ANPC* NPC)
{
	UResourceComponent* EnergyComp = NPC->GetResourceComponent<EResourceType::Energy>();
	if (!EnergyComp)
		return false;
	
	EnergyComp->SetIsInRegen(true);
	
	UResourceComponent* FoodComp = NPC->GetResourceComponent<EResourceType::Food>();
	if (!FoodComp)
		return false;
	
	FoodComp->ToggleResourceTimer(); 
	
	return true;
}

bool ABuildableBed::StopUsingImplementation(ANPC* NPC)
{
	UResourceComponent* EnergyComp = NPC->GetResourceComponent<EResourceType::Energy>();
	if (!EnergyComp)
		return false;

	EnergyComp->SetIsInRegen(false);
	
	UResourceComponent* FoodComp = NPC->GetResourceComponent<EResourceType::Food>();
	if (!FoodComp)
		return false;
	
	FoodComp->ToggleResourceTimer(); 
	
	return true;
}
#pragma endregion

#pragma region Selectable
void ABuildableBed::OnSelected()
{
}

void ABuildableBed::OnDeselected()
{
}
#pragma endregion
