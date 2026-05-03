#include "Object/Buildable/BuildableGenerator.h"

#include "Build/BuildableRegistrySubsystem.h"
#include "Game/CrowdedGameState.h"
#include "Player/CrowdedPlayerController.h"

ABuildableGenerator::ABuildableGenerator()
{
	ProductionComponent = CreateDefaultSubobject<UProductionComponent>("ProductionComponent");
	
	SelectionType = ESelectionType::Generator;
	ObjectType = EObjectType::Generator;
	
	NPCUseAction = ENPCActionType::Work;
	NPCMoveAction = ENPCActionType::MoveToWork;
	
	UsingResourceTypeToCheck = EResourceType::None;
}

void ABuildableGenerator::BeginPlay()
{
	Super::BeginPlay();

	// Register generator
	if (!BRS)
		return;

	BRS->RegisterGenerator(this);

	// Get player money component
	PlayerMoneyComponent = GameState->GetResourceComponent<EResourceType::Money>();

	// Assign start production values
	if (!ProductionComponent)
		return;

	if (!ProductionUpgradeData)
		return;

	ProductionComponent->SetProductionType(ProductionUpgradeData->ProductionType);
	ProductionComponent->SetProductionInterval(ProductionUpgradeData->StartProductionInterval);
	ProductionComponent->SetResourcePerInterval(ProductionUpgradeData->StartResourcePerInterval);

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

#pragma region Use Object
bool ABuildableGenerator::StartUsingImplementation(ANPC* NPC)
{
	ProductionComponent->SetProductionMultiplier(NPC->GetProductionMultiplierForType(ProductionComponent->GetProductionType()));
	ProductionComponent->StartProduction();
	return true; 
}

bool ABuildableGenerator::StopUsingImplementation(ANPC* NPC)
{
	ProductionComponent->SetProductionMultiplier(1);
	ProductionComponent->PauseProduction();
	return true; 
}
#pragma endregion

#pragma region Upgrade
EProductionType ABuildableGenerator::GetProductionType() const
{
	return ProductionComponent->GetProductionType();
}

void ABuildableGenerator::OnNextUpgrade()
{
	if (!bHasNextUpgrade)
		return;
	
	// Upgrade stats
	ProductionComponent->SetProductionInterval(ProductionUpgradeData->UpgradesInOrder[CurrentUpgrade].UpgradeProductionInterval);
	ProductionComponent->SetResourcePerInterval(ProductionUpgradeData->UpgradesInOrder[CurrentUpgrade].UpgradeResourcePerInterval);
	
	ProductionComponent->OnProductionIntervalChanged.Broadcast(ProductionComponent->GetProductionInterval());
	ProductionComponent->OnResourcePerIntervalChanged.Broadcast(ProductionComponent->GetResourcePerInterval());
	
	// Cost
	PlayerMoneyComponent->RemoveResource(ProductionUpgradeData->UpgradesInOrder[CurrentUpgrade].UpgradeCost);

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

UResourceComponent* ABuildableGenerator::GetPlayerMoneyComponent() const
{
	return PlayerMoneyComponent;
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

#pragma region Actions
void ABuildableGenerator::InitActions()
{
	Super::InitActions();

	if (!GameState)
		return;
	
	TArray<TObjectPtr<UAction>> InstancedActions;

	for (const TSubclassOf<UAction>& ActionClass : GameState->GeneratorActions)
	{
		if (!ActionClass) continue;

		UAction* NewAction = NewObject<UAction>(this, ActionClass);
		if (!NewAction) continue;

		NewAction->Initialize(GetWorld());
		InstancedActions.Add(NewAction);
	}

	ActionComponent->SetupActions(InstancedActions);
}
#pragma endregion 
