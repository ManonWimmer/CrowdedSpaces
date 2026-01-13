#include "Resources/OxygenComponent.h"

UOxygenComponent::UOxygenComponent()
{
}

void UOxygenComponent::AddOxygen(int Amount)
{
	Oxygen += Amount;
	OnStatChanged.Broadcast("Oxygen", Oxygen);
}

void UOxygenComponent::RemoveOxygen(int Amount)
{
	if (!HasEnoughOxygen(Amount)) return;
	
	Oxygen -= Amount;
	OnStatChanged.Broadcast("Oxygen", Oxygen);
}

bool UOxygenComponent::HasEnoughOxygen(int Amount)
{
	return Oxygen >= Amount;
}

#pragma region Selectable
TArray<TPair<FString, FString>> UOxygenComponent::GetCurrentValues() const
{
	TArray<TPair<FString, FString>> values;
	values.Add(TPair<FString, FString>(FString("Oxygen"), FString::SanitizeFloat(Oxygen)));
	return values;
}
#pragma endregion Selectable


