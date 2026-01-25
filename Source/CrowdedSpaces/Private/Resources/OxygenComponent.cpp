#include "Resources/OxygenComponent.h"

UOxygenComponent::UOxygenComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UOxygenComponent::AddOxygen(int Amount)
{
	Oxygen += Amount;
	OnStatChanged.Broadcast("Oxygen", FString::SanitizeFloat(Oxygen));
}

void UOxygenComponent::RemoveOxygen(int Amount)
{
	if (!HasEnoughOxygen(Amount)) return;
	
	Oxygen -= Amount;
	OnStatChanged.Broadcast("Oxygen", FString::SanitizeFloat(Oxygen));
}

bool UOxygenComponent::HasEnoughOxygen(int Amount)
{
	return Oxygen >= Amount;
}

#pragma region Selectable
TArray<TPair<FString, FString>> UOxygenComponent::GetCurrentValues() const
{
	TArray<TPair<FString, FString>> Values;
	Values.Add(TPair<FString, FString>(FString("Oxygen"), FString::SanitizeFloat(Oxygen)));
	return Values;
}
#pragma endregion Selectable


