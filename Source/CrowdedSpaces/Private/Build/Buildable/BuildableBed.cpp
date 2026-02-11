#include "Build/Buildable/BuildableBed.h"

#include "Build/BuildableRegistrySubsystem.h"

ABuildableBed::ABuildableBed()
{
	SelectionType = ESelectionType::Bed;
	ObjectType = EObjectType::Bed;
}

void ABuildableBed::BeginPlay()
{
	Super::BeginPlay();

	TObjectPtr<UBuildableRegistrySubsystem> BRS = GetWorld()->GetSubsystem<UBuildableRegistrySubsystem>();
	if (!BRS)
		return;

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Register bed");

	BRS->RegisterBed(this);
}

void ABuildableBed::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	TObjectPtr<UBuildableRegistrySubsystem> BRS = GetWorld()->GetSubsystem<UBuildableRegistrySubsystem>();
	if (!BRS)
		return;

	BRS->UnregisterBed(this);
}

void ABuildableBed::StartUsingImplementation()
{
	Super::StartUsingImplementation();
}

void ABuildableBed::StopUsingImplementation()
{
	Super::StopUsingImplementation();
}

#pragma region Selectable
void ABuildableBed::OnSelected()
{
}

void ABuildableBed::OnDeselected()
{
}
#pragma endregion

