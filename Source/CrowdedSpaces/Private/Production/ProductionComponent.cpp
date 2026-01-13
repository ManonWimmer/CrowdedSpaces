#include "Production/ProductionComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Player/CrowdedPlayerController.h"
#include "Player/CrowdedPlayerState.h"

UProductionComponent::UProductionComponent(): PlayerMoneyComponent(nullptr)
{
}

#pragma region Selectable
TArray<TPair<FString, FString>> UProductionComponent::GetCurrentValues() const
{
	TArray<TPair<FString, FString>> values;
	FString ProductionTypeName = UEnum::GetValueAsString(ProductionType);
	ProductionTypeName.RemoveFromStart(TEXT("EProductionType::"));
	values.Add(TPair<FString, FString>(FString("Production Type"), ProductionTypeName)); 
	values.Add(TPair<FString, FString>(FString("Production Interval"), FString::SanitizeFloat(ProductionInterval)));
	values.Add(TPair<FString, FString>(FString("Resource Per Interval"), FString::SanitizeFloat(ResourcePerInterval)));
	return values;
}
#pragma endregion Selectable

void UProductionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (ACrowdedPlayerController* CamPC = Cast<ACrowdedPlayerController>(PC))
		{
			// Money component
			if (ACrowdedPlayerState* PS = PC->GetPlayerState<ACrowdedPlayerState>())
			{
				PlayerMoneyComponent = PS->GetMoneyComponent();
			}
		}
	}
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
}

void UProductionComponent::PauseProduction() const
{
	if (GetOwner()->GetWorldTimerManager().IsTimerActive(ProductionTimerHandle))
	{
		GetOwner()->GetWorldTimerManager().PauseTimer(ProductionTimerHandle);
	}
}

void UProductionComponent::ResumeProduction() const
{
	if (GetOwner()->GetWorldTimerManager().IsTimerPaused(ProductionTimerHandle))
	{
		GetOwner()->GetWorldTimerManager().UnPauseTimer(ProductionTimerHandle);
	}
}

void UProductionComponent::RestartProduction()
{
	GetOwner()->GetWorldTimerManager().ClearTimer(ProductionTimerHandle);

	StartProduction();
}



