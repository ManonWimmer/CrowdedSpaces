#include "Build/Buildable/BuildableGenerator.h"

#include "Build/BuildableRegistrySubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CrowdedPlayerController.h"
#include "Player/CrowdedPlayerState.h"

ABuildableGenerator::ABuildableGenerator()
{
	ProductionComponent = CreateDefaultSubobject<UProductionComponent>("ProductionComponent");
	
	SelectionType = ESelectionType::Generator;
	ObjectType = EObjectType::Generator;
	NPCAction = ENPCAction::Work;
}

void ABuildableGenerator::BeginPlay()
{
	Super::BeginPlay();

	// Register generator
	if (!BRS)
		return;

	BRS->RegisterGenerator(this);

	// Get player money component
	
	if (!CrowdedPlayerState)
		return;
	
	PlayerMoneyComponent = CrowdedPlayerState->GetMoneyComponent();

	// Assign start production values
	if (!ProductionComponent)
		return;

	if (!ProductionUpgradeData)
		return;

	ProductionComponent->ProductionType = ProductionUpgradeData->ProductionType;
	ProductionComponent->ProductionInterval = ProductionUpgradeData->StartProductionInterval;
	ProductionComponent->ResourcePerInterval = ProductionUpgradeData->StartResourcePerInterval;

	if (ProductionUpgradeData->UpgradesInOrder.Num() > 0)
	{
		bHasNextUpgrade = true;
		NextUpgrade = ProductionUpgradeData->UpgradesInOrder[CurrentUpgrade];
	}
	else
	{
		bHasNextUpgrade = false;
	}
}

void ABuildableGenerator::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	if (!BRS)
		return;

	BRS->UnregisterGenerator(this);
}

bool ABuildableGenerator::StartUsingImplementation(UBTTask_UseBuildableObject* UseObjectTask)
{
	CurrentTasks.Add(UseObjectTask);
	
	ProductionComponent->StartProduction();
	return true; 
}

bool ABuildableGenerator::StopUsingImplementation(UBTTask_UseBuildableObject* UseObjectTask)
{
	CurrentTasks.Remove(UseObjectTask);
	
	ProductionComponent->PauseProduction();
	return true; 
}

#pragma region Upgrade
void ABuildableGenerator::OnNextUpgrade()
{
	if (!bHasNextUpgrade)
		return;
	
	// Upgrade stats
	ProductionComponent->ProductionInterval = ProductionUpgradeData->UpgradesInOrder[CurrentUpgrade].UpgradeProductionInterval;
	ProductionComponent->ResourcePerInterval = ProductionUpgradeData->UpgradesInOrder[CurrentUpgrade].UpgradeResourcePerInterval;
	
	ProductionComponent->OnProductionIntervalChanged.Broadcast(ProductionComponent->ProductionInterval);
	ProductionComponent->OnResourcePerIntervalChanged.Broadcast(ProductionComponent->ResourcePerInterval);
	
	// Cost
	PlayerMoneyComponent->RemoveMoney(ProductionUpgradeData->UpgradesInOrder[CurrentUpgrade].UpgradeCost);

	// Check next upgrade
	CurrentUpgrade++; 
	
	if (CurrentUpgrade <= ProductionUpgradeData->UpgradesInOrder.Num() - 1)
	{
		bHasNextUpgrade = true;
		NextUpgrade = ProductionUpgradeData->UpgradesInOrder[CurrentUpgrade];
	}
	else
	{
		bHasNextUpgrade = false;
	}
}

bool ABuildableGenerator::GetHasNextUpgrade()
{
	return bHasNextUpgrade;
}

FUpgradeStruct ABuildableGenerator::GetNextUpgrade()
{
	return NextUpgrade;
}
#pragma endregion Upgrade

#pragma region Selectable
void ABuildableGenerator::OnSelected()
{
}

void ABuildableGenerator::OnDeselected()
{
}
#pragma endregion Selectable

