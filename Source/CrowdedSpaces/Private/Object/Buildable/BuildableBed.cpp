#include "Object/Buildable/BuildableBed.h"

#include "Build/BuildableRegistrySubsystem.h"
#include "Components/CapsuleComponent.h"

ABuildableBed::ABuildableBed()
{
	SelectionType = ESelectionType::Bed;
	ObjectType = EObjectType::Bed;
	
	NPCUseAction = ENPCActionType::Sleep;
	NPCMoveAction = ENPCActionType::MoveToBed;
	
	UsingResourceTypeToCheck = EResourceType::Energy;
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

	NPC->bIsInSleepAnimation = true;

	// Désactive collision entre NPC et Bed
	NPC->GetCapsuleComponent()->IgnoreActorWhenMoving(this, true);
	this->GetComponentByClass<UPrimitiveComponent>()->IgnoreActorWhenMoving(NPC, true);
	
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

	NPC->bIsInSleepAnimation = false;
	
	// Réactive collision
	NPC->GetCapsuleComponent()->IgnoreActorWhenMoving(this, false);
	this->GetComponentByClass<UPrimitiveComponent>()->IgnoreActorWhenMoving(NPC, false);
	
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
