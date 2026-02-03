#include "Production/ProductionComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Player/CrowdedPlayerController.h"
#include "Player/CrowdedPlayerState.h"

UProductionComponent::UProductionComponent(): PlayerMoneyComponent(nullptr), PlayerElectricityComponent(nullptr),
											  PlayerOxygenComponent(nullptr), PlayerFoodComponent(nullptr)
{
}

#pragma region Selectable
TArray<TPair<FString, FString>> UProductionComponent::GetCurrentValues() const
{
	TArray<TPair<FString, FString>> Values;
	FString ProductionTypeName = UEnum::GetValueAsString(ProductionType);
	ProductionTypeName.RemoveFromStart(TEXT("EProductionType::"));
	Values.Add(TPair<FString, FString>(FString("Production Type"), ProductionTypeName));

	FString Active = GetOwner()->GetWorldTimerManager().IsTimerActive(ProductionTimerHandle) ? TEXT("True") : TEXT("False");
	Values.Add(TPair<FString, FString>(FString("Is Active"), Active));
	
	Values.Add(TPair<FString, FString>(FString("Production Interval"), FString::SanitizeFloat(ProductionInterval)));
	Values.Add(TPair<FString, FString>(FString("Resource Per Interval"), FString::SanitizeFloat(ResourcePerInterval)));
	return Values;
}
#pragma endregion Selectables

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
	PlayerMoneyComponent = PS->GetMoneyComponent();
	PlayerElectricityComponent = PS->GetElectricityComponent();
	PlayerOxygenComponent = PS->GetOxygenComponent();
	PlayerFoodComponent = PS->GetFoodComponent();
}

void UProductionComponent::GenerateProduction() const
{
	switch (ProductionType)
	{
		// Money
		case EProductionType::Money:
			if (PlayerMoneyComponent)
			{
				PlayerMoneyComponent->AddMoney(ResourcePerInterval);
			}
			break;

		// Electricity
		case EProductionType::Electricity:
			if (PlayerElectricityComponent)
			{
				PlayerElectricityComponent->AddElectricity(ResourcePerInterval);
			}
			break;

		// Oxygen
		case EProductionType::Oxygen:
			if (PlayerOxygenComponent)
			{
				PlayerOxygenComponent->AddOxygen(ResourcePerInterval);
			}
			break;

		// Food
		case EProductionType::Food:
			if (PlayerFoodComponent)
			{
				PlayerFoodComponent->AddFood(ResourcePerInterval);
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

	OnStatChanged.Broadcast("Is Active", "True");
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

	OnStatChanged.Broadcast("Is Active", "False");
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

	OnStatChanged.Broadcast("Is Active", "False");
}

void UProductionComponent::RestartProduction()
{
	GetOwner()->GetWorldTimerManager().ClearTimer(ProductionTimerHandle);

	StartProduction();
}



