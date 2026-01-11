#include "Build/BuildableGenerator.h"

#include "Kismet/GameplayStatics.h"
#include "Player/CrowdedPlayerController.h"
#include "Player/CrowdedPlayerState.h"

ABuildableGenerator::ABuildableGenerator(): MoneyComponent(nullptr)
{
}

void ABuildableGenerator::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (ACrowdedPlayerController* CamPC = Cast<ACrowdedPlayerController>(PC))
		{
			// Money component
			if (ACrowdedPlayerState* PS = PC->GetPlayerState<ACrowdedPlayerState>())
			{
				MoneyComponent = PS->GetMoneyComponent();
			}
		}
	}
	
	StartProduction();
}

void ABuildableGenerator::GenerateProduction() const
{
	switch (ProductionType)
	{
		// Money
		case EProductionType::Money:
			if (MoneyComponent)
			{
				MoneyComponent->AddMoney(ResourcePerInterval);
			}
			break;
		
		default:
			break;
	}
}

void ABuildableGenerator::StartProduction()
{
	// Start production (timer + optimisé que tick)
	GetWorldTimerManager().SetTimer(
		ProductionTimerHandle,
		this,
		&ABuildableGenerator::GenerateProduction,
		ProductionInterval,
		true // looping
	);
}

void ABuildableGenerator::PauseProduction() const
{
	if (GetWorldTimerManager().IsTimerActive(ProductionTimerHandle))
	{
		GetWorldTimerManager().PauseTimer(ProductionTimerHandle);
	}
}

void ABuildableGenerator::ResumeProduction() const
{
	if (GetWorldTimerManager().IsTimerPaused(ProductionTimerHandle))
	{
		GetWorldTimerManager().UnPauseTimer(ProductionTimerHandle);
	}
}

void ABuildableGenerator::RestartProduction()
{
	GetWorldTimerManager().ClearTimer(ProductionTimerHandle);

	StartProduction();
}

