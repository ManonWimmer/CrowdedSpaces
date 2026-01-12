#include "Resources/MoneyComponent.h"

UMoneyComponent::UMoneyComponent()
{
}

void UMoneyComponent::AddMoney(int Amount)
{
	Money += Amount;
	OnStatChanged.Broadcast("Money", Money);
}

void UMoneyComponent::RemoveMoney(int Amount)
{
	if (!HasEnoughMoney(Amount)) return;
	
	Money -= Amount;
	OnStatChanged.Broadcast("Money", Money);
}

bool UMoneyComponent::HasEnoughMoney(int Amount)
{
	return Money >= Amount;
}
