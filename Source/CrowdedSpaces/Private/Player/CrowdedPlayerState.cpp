#include "Player/CrowdedPlayerState.h"

ACrowdedPlayerState::ACrowdedPlayerState()
{
	MoneyComponent = CreateDefaultSubobject<UResourceComponent>(TEXT("MoneyComponent"));
	MoneyComponent->SetType(EResourceType::Money);
	MoneyComponent->SetCanLoseAndRegenResource(false);
	
	ElectricityComponent = CreateDefaultSubobject<UResourceComponent>(TEXT("ElectricityComponent"));
	ElectricityComponent->SetType(EResourceType::Electricity);
	ElectricityComponent->SetCanLoseAndRegenResource(false);
	
	OxygenComponent = CreateDefaultSubobject<UResourceComponent>(TEXT("OxygenComponent"));
	OxygenComponent->SetType(EResourceType::Oxygen);
	OxygenComponent->SetCanLoseAndRegenResource(false);
	
	FoodComponent = CreateDefaultSubobject<UResourceComponent>(TEXT("FoodComponent"));
	FoodComponent->SetType(EResourceType::Food);
	FoodComponent->SetCanLoseAndRegenResource(false);
}
