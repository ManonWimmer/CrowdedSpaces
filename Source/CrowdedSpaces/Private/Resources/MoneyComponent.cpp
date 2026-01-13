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

#pragma region Selectable
TArray<TPair<FString, float>> UMoneyComponent::GetCurrentValues() const
{
	TArray<TPair<FString, float>> values;
	values.Add(TPair<FString, float>(FString("Money"), Money));
	return values;
}
#pragma endregion Selectable
