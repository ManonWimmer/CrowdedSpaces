#include "Production/ProductionComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Player/CrowdedPlayerController.h"
#include "Player/CrowdedPlayerState.h"

UProductionComponent::UProductionComponent(): PlayerMoneyComponent(nullptr), PlayerElectricityComponent(nullptr), PlayerFoodComponent(nullptr)
{
}

void UProductionComponent::BeginPlay()
{
	Super::BeginPlay();

	TObjectPtr<APlayerController> PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC)
		return;

	TObjectPtr<ACrowdedPlayerController> CamPC = Cast<ACrowdedPlayerController>(PC);
	if (!CamPC)
		return;

	TObjectPtr<ACrowdedPlayerState> PS = PC->GetPlayerState<ACrowdedPlayerState>();
	if (!PS)
		return;
	
	// Get player components
	PlayerMoneyComponent = PS->GetResourceComponent<EResourceType::Money>();
	PlayerElectricityComponent = PS->GetResourceComponent<EResourceType::Electricity>();
	PlayerFoodComponent = PS->GetResourceComponent<EResourceType::Food>();
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



