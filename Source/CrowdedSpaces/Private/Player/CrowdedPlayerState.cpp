#include "Player/CrowdedPlayerState.h"

ACrowdedPlayerState::ACrowdedPlayerState()
{
	MoneyComponent = CreateDefaultSubobject<UMoneyComponent>(TEXT("MoneyComponent"));
}
