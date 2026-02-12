#include "Build/Buildable/BuildableFood.h"

#include "Build/BuildableRegistrySubsystem.h"

ABuildableFood::ABuildableFood()
{
	SelectionType = ESelectionType::Food;
	ObjectType = EObjectType::Food;
	NPCAction = ENPCAction::Eat;
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

bool ABuildableFood::StartUsingImplementation(UBTTask_UseBuildableObject* UseObjectTask)
{
	CurrentUsers.Add(UseObjectTask);
	
	UFoodComponent* FoodComp = UsingNPC->FindComponentByClass<UFoodComponent>();
	if (!FoodComp)
		return false;
	
	FoodComp->SetEating(true);
	
	FoodComp->OnFoodFull.AddDynamic(UseObjectTask, &UBTTask_UseBuildableObject::OnStopAction);

	return true;
}

bool ABuildableFood::StopUsingImplementation(UBTTask_UseBuildableObject* UseObjectTask)
{
	CurrentUsers.Remove(UseObjectTask);
	
	UFoodComponent* FoodComp = UsingNPC->FindComponentByClass<UFoodComponent>();
	if (!FoodComp)
		return false;

	FoodComp->SetEating(false);

	FoodComp->OnFoodFull.RemoveDynamic(UseObjectTask, &UBTTask_UseBuildableObject::OnStopAction);
	
	return true;
}

#pragma region Selectable
void ABuildableFood::OnSelected()
{
}

void ABuildableFood::OnDeselected()
{
}
#pragma endregion


