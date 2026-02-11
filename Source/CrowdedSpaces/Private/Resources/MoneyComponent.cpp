#include "Resources/MoneyComponent.h"

UMoneyComponent::UMoneyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMoneyComponent::AddMoney(int Amount)
{
	Money += Amount;
	OnMoneyChanged.Broadcast(Money);
}

void UMoneyComponent::RemoveMoney(int Amount)
{
	if (!HasEnoughMoney(Amount)) return;
	
	Money -= Amount;
	OnMoneyChanged.Broadcast(Money);
}

bool UMoneyComponent::HasEnoughMoney(int Amount)
{
	return Money >= Amount;
}
