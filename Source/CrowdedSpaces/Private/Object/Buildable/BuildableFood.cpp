#include "Object/Buildable/BuildableFood.h"

#include "Build/BuildableRegistrySubsystem.h"

ABuildableFood::ABuildableFood()
{
	SelectionType = ESelectionType::Food;
	ObjectType = EObjectType::Food;
	
	NPCUseAction = ENPCActionType::Eat;
	NPCMoveAction = ENPCActionType::MoveToFood;
	
	UsingResourceTypeToCheck = EResourceType::Food;
}

void ABuildableFood::BeginPlay()
{
	Super::BeginPlay();
	
	if (!BRS)
		return;

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Register food");

	BRS->RegisterFood(this);
}

void ABuildableFood::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (!BRS)
		return;
	
	BRS->UnregisterFood(this);
}

#pragma region Use Object
bool ABuildableFood::StartUsingImplementation(ANPC* NPC)
{
	UResourceComponent* FoodComp = NPC->GetResourceComponent<EResourceType::Food>();
	if (!FoodComp)
		return false;
	
	FoodComp->SetIsInRegen(true);

	UResourceComponent* EnergyComp = NPC->GetResourceComponent<EResourceType::Energy>();
	if (!EnergyComp)
		return false;
	
	EnergyComp->ToggleResourceTimer();

	NPC->bIsInEatingAnimation = true;
	
	return true;
}

bool ABuildableFood::StopUsingImplementation(ANPC* NPC)
{
	UResourceComponent* FoodComp = NPC->GetResourceComponent<EResourceType::Food>();
	if (!FoodComp)
		return false;

	FoodComp->SetIsInRegen(false);
	
	UResourceComponent* EnergyComp = NPC->GetResourceComponent<EResourceType::Energy>();
	if (!EnergyComp)
		return false;
	
	EnergyComp->ToggleResourceTimer();
	
	NPC->bIsInEatingAnimation = false;
	
	return true;
}
#pragma endregion

#pragma region Selectable
void ABuildableFood::OnSelected()
{
}

void ABuildableFood::OnDeselected()
{
}
#pragma endregion
