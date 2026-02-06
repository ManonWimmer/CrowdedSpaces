#include "Build/Buildable/BuildableGenerator.h"

#include "Build/BuildableRegistrySubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CrowdedPlayerController.h"
#include "Player/CrowdedPlayerState.h"

ABuildableGenerator::ABuildableGenerator()
{
	ProductionComponent = CreateDefaultSubobject<UProductionComponent>("ProductionComponent");
	
	// Selectable
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

	// Get player money component
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC)
		return;

	ACrowdedPlayerController* CamPC = Cast<ACrowdedPlayerController>(PC);
	if (!CamPC)
		return;
	
	if (ACrowdedPlayerState* PS = PC->GetPlayerState<ACrowdedPlayerState>())
	{
		PlayerMoneyComponent = PS->GetMoneyComponent();
	}

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

	UBuildableRegistrySubsystem* BRS = GetWorld()->GetSubsystem<UBuildableRegistrySubsystem>();
	if (!BRS)
		return;

	BRS->UnregisterGenerator(this);
}

#pragma region Work
void ABuildableGenerator::SetNPCWorking(bool bWorking)
{
	bHasNPCWorking = bWorking;

	if (bHasNPCWorking)
		ProductionComponent->ResumeOrStartProduction();
	else
		ProductionComponent->PauseProduction();
}
#pragma endregion Work

#pragma region Upgrade
void ABuildableGenerator::OnNextUpgrade()
{
	if (!bHasNextUpgrade)
		return;
	
	// Upgrade stats
	ProductionComponent->ProductionInterval = ProductionUpgradeData->UpgradesInOrder[CurrentUpgrade].UpgradeProductionInterval;
	ProductionComponent->ResourcePerInterval = ProductionUpgradeData->UpgradesInOrder[CurrentUpgrade].UpgradeResourcePerInterval;
	
	ProductionComponent->OnStatChanged.Broadcast("Production Interval", FString::SanitizeFloat(ProductionComponent->ProductionInterval));
	ProductionComponent->OnStatChanged.Broadcast("Resource Per Interval", FString::SanitizeFloat(ProductionComponent->ResourcePerInterval));
	
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

FString ABuildableGenerator::GetDisplayName() const
{
	return "Generator";
}

TObjectPtr<AActor> ABuildableGenerator::GetSelectableActor()
{
	return this;
}
#pragma endregion Selectable

