#include "Build/Buildable/BuildableGenerator.h"

#include "Build/BuildableRegistrySubsystem.h"

ABuildableGenerator::ABuildableGenerator()
{
	ProductionComponent = CreateDefaultSubobject<UProductionComponent>("ProductionComponent");
	SelectionType = ESelectionType::Generator;
}

void ABuildableGenerator::BeginPlay()
{
	Super::BeginPlay();

	// Register generator
	UBuildableRegistrySubsystem* BRS = GetWorld()->GetSubsystem<UBuildableRegistrySubsystem>();
	if (!BRS)
		return;
		
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Register generator");

	BRS->RegisterGenerator(this);

	// Assign start production values
	if (!ProductionComponent)
		return;

	if (!ProductionUpgradeData)
		return;

	ProductionComponent->ProductionType = ProductionUpgradeData->ProductionType;
	ProductionComponent->ProductionInterval = ProductionUpgradeData->StartProductionInterval;
	ProductionComponent->ResourcePerInterval = ProductionUpgradeData->StartResourcePerInterval;
}

void ABuildableGenerator::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UBuildableRegistrySubsystem* BRS = GetWorld()->GetSubsystem<UBuildableRegistrySubsystem>();
	if (!BRS)
		return;

	BRS->UnregisterGenerator(this);
}

void ABuildableGenerator::SetNPCWorking(bool bWorking)
{
	bHasNPCWorking = bWorking;

	if (bHasNPCWorking)
		ProductionComponent->ResumeOrStartProduction();
	else
		ProductionComponent->PauseProduction();
}

#pragma region Selectable
void ABuildableGenerator::OnSelected()
{
}

void ABuildableGenerator::OnDeselected()
{
}

FString ABuildableGenerator::GetDisplayName() const
{
	return "Generator";
}

AActor* ABuildableGenerator::GetSelectableActor()
{
	return this;
}
#pragma endregion Selectable

