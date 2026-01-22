#include "Player/CrowdedPlayerState.h"

ACrowdedPlayerState::ACrowdedPlayerState()
{
	MoneyComponent = CreateDefaultSubobject<UMoneyComponent>(TEXT("MoneyComponent"));
	ElectricityComponent = CreateDefaultSubobject<UElectricityComponent>(TEXT("ElectricityComponent"));
	OxygenComponent = CreateDefaultSubobject<UOxygenComponent>(TEXT("OxygenComponent"));
	FoodComponent = CreateDefaultSubobject<UFoodComponent>(TEXT("FoodComponent"));
}
