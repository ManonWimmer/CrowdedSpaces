#include "Production/ProductionComponent.h"

#include "Game/CrowdedGameState.h"
#include "Player/CrowdedPlayerController.h"

UProductionComponent::UProductionComponent(): PlayerMoneyComponent(nullptr), PlayerElectricityComponent(nullptr), PlayerFoodComponent(nullptr)
{
}

void UProductionComponent::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (!World)
		return;

	TObjectPtr<ACrowdedGameState> GameState = World->GetGameState<ACrowdedGameState>();
	if (!GameState)
		return;
	
	// Get player components
	PlayerMoneyComponent = GameState->GetResourceComponent<EResourceType::Money>();
	PlayerElectricityComponent = GameState->GetResourceComponent<EResourceType::Electricity>();
	PlayerFoodComponent = GameState->GetResourceComponent<EResourceType::Food>();
}

void UProductionComponent::GenerateProduction() const
{
	switch (ProductionType)
	{
		// Money
		case EProductionType::Money:
			if (PlayerMoneyComponent)
			{
				PlayerMoneyComponent->AddResource(ResourcePerInterval * ProductionMultiplier);
			}
			break;

		// Electricity
		case EProductionType::Electricity:
			if (PlayerElectricityComponent)
			{
				PlayerElectricityComponent->AddResource(ResourcePerInterval * ProductionMultiplier);
			}
			break;

		// Food
		case EProductionType::Food:
			if (PlayerFoodComponent)
			{
				PlayerFoodComponent->AddResource(ResourcePerInterval * ProductionMultiplier);
			}
			break;
		
		default:
			break;
	}
}

void UProductionComponent::StartProduction()
{
	// Start production (timer + optimisé que tick)
	GetOwner()->GetWorldTimerManager().SetTimer(
		ProductionTimerHandle,
		this,
		&UProductionComponent::GenerateProduction,
		ProductionInterval,
		true // looping
	);

	bHasStartedProduction = true;

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, "Production Started");

	OnIsActiveChanged.Broadcast(true);
}

void UProductionComponent::PauseProduction()
{
	if (!bHasStartedProduction)
		return;

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, "Production Paused");
	
	if (GetOwner()->GetWorldTimerManager().IsTimerActive(ProductionTimerHandle))
	{
		GetOwner()->GetWorldTimerManager().PauseTimer(ProductionTimerHandle);
	}

	OnIsActiveChanged.Broadcast(false);
}

void UProductionComponent::ResumeOrStartProduction()
{
	if (!bHasStartedProduction)
	{
		StartProduction();
		return;
	}

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, "Production UnPaused");
	
	if (GetOwner()->GetWorldTimerManager().IsTimerPaused(ProductionTimerHandle))
	{
		GetOwner()->GetWorldTimerManager().UnPauseTimer(ProductionTimerHandle);
	}

	OnIsActiveChanged.Broadcast(false);
}

void UProductionComponent::RestartProduction()
{
	GetOwner()->GetWorldTimerManager().ClearTimer(ProductionTimerHandle);

	StartProduction();
}



