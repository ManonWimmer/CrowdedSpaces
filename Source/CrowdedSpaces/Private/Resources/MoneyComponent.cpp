#include "Resources/MoneyComponent.h"

UMoneyComponent::UMoneyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
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
TArray<TPair<FString, FString>> UMoneyComponent::GetCurrentValues() const
{
	TArray<TPair<FString, FString>> Values;
	Values.Add(TPair<FString, FString>(FString("Money"), FString::SanitizeFloat(Money)));
	return Values;
}
#pragma endregion Selectable
