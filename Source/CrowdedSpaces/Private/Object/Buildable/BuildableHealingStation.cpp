#include "Object/Buildable/BuildableHealingStation.h"

#include "Build/BuildableRegistrySubsystem.h"
#include "Debug/CrowdedSpacesLogs.h"

ABuildableHealingStation::ABuildableHealingStation()
{
	SelectionType = ESelectionType::HealingStation;
	ObjectType = EObjectType::HealingStation;
	NPCUseAction = ENPCActionType::Heal;
	NPCMoveAction = ENPCActionType::MoveToHealingStation;
}

void ABuildableHealingStation::BeginPlay()
{
	Super::BeginPlay();

	if (!BRS)
		return;
	
	//BRS->RegisterBed(this);
}

void ABuildableHealingStation::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (!BRS)
		return;

	//BRS->UnregisterBed(this);
}

#pragma region Use Object
bool ABuildableHealingStation::StartUsingImplementation(ANPC* NPC)
{
	// TODO : health component ici au lieu de damageable ?

	CS_LOG("Start using healing station");
	
	return true;
}

bool ABuildableHealingStation::StopUsingImplementation(ANPC* NPC)
{
	CS_LOG("Stop using healing station");
	
	return true;
}
#pragma endregion

#pragma region Selectable
void ABuildableHealingStation::OnSelected()
{
}

void ABuildableHealingStation::OnDeselected()
{
}
#pragma endregion
