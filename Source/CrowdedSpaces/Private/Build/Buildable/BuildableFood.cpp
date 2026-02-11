#include "Build/Buildable/BuildableFood.h"

#include "Build/BuildableRegistrySubsystem.h"

ABuildableFood::ABuildableFood()
{
	SelectionType = ESelectionType::Food;
	ObjectType = EObjectType::Food;
}

void ABuildableFood::BeginPlay()
{
	Super::BeginPlay();

	UBuildableRegistrySubsystem* BRS = GetWorld()->GetSubsystem<UBuildableRegistrySubsystem>();
	if (!BRS)
		return;

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Register food");

	BRS->RegisterFood(this);
}

void ABuildableFood::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UBuildableRegistrySubsystem* BRS = GetWorld()->GetSubsystem<UBuildableRegistrySubsystem>();
	if (!BRS)
		return;
	
	BRS->UnregisterFood(this);
}

void ABuildableFood::StartUsingImplementation()
{
	Super::StartUsingImplementation();
}

void ABuildableFood::StopUsingImplementation()
{
	Super::StopUsingImplementation();
}

#pragma region Selectable
void ABuildableFood::OnSelected()
{
}

void ABuildableFood::OnDeselected()
{
}
#pragma endregion


