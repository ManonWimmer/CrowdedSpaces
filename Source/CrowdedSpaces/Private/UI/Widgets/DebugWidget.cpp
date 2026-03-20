#include "UI/Widgets/DebugWidget.h"

#include "AI/NPCFunctionLibrary.h"
#include "AI/NPC.h"
#include "Game/CrowdedGameState.h"

void UDebugWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UWorld* World = GetWorld();
	if (!World)
		return;

	const TObjectPtr<ACrowdedGameState> GameState = World->GetGameState<ACrowdedGameState>();
	if (!GameState)
		return;

	MoneyComponent = GameState->GetResourceComponent<EResourceType::Money>();
	ElectricityComponent = GameState->GetResourceComponent<EResourceType::Electricity>();
	FoodComponent = GameState->GetResourceComponent<EResourceType::Food>();
}

void UDebugWidget::Reset_Implementation()
{
	Super::Reset_Implementation();
}

void UDebugWidget::Setup_Implementation(AActor* SelectableActor)
{
	Super::Setup_Implementation(SelectableActor);
}

void UDebugWidget::Unsetup_Implementation()
{
	Super::Unsetup_Implementation();
}

void UDebugWidget::AddMoney(const int Amount) const
{
	if (!MoneyComponent)
		return;

	MoneyComponent->AddResource(Amount);
}

void UDebugWidget::AddElectricity(const int Amount) const
{
	if (!ElectricityComponent)
		return;

	ElectricityComponent->AddResource(Amount);
}

void UDebugWidget::AddFood(const int Amount) const
{
	if (!FoodComponent)
		return;

	FoodComponent->AddResource(Amount);
}

void UDebugWidget::SpawnNPC()
{
	ANPC* SpawnedNPC = UNPCFunctionLibrary::SpawnGlobalNPC(this);
}

void UDebugWidget::Init_Implementation()
{
	Super::Init_Implementation();
}

