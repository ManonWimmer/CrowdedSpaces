#include "Resources/ElectricityComponent.h"

UElectricityComponent::UElectricityComponent()
{
}

void UElectricityComponent::AddElectricity(int Amount)
{
	Electricity += Amount;
	OnStatChanged.Broadcast("Electricity", Electricity);
}

void UElectricityComponent::RemoveElectricity(int Amount)
{
	if (!HasEnoughElectricity(Amount)) return;
	
	Electricity -= Amount;
	OnStatChanged.Broadcast("Electricity", Electricity);
}

bool UElectricityComponent::HasEnoughElectricity(int Amount)
{
	return Electricity >= Amount;
}

#pragma region Selectable
TArray<TPair<FString, FString>> UElectricityComponent::GetCurrentValues() const
{
	TArray<TPair<FString, FString>> values;
	values.Add(TPair<FString, FString>(FString("Electricity"), FString::SanitizeFloat(Electricity)));
	return values;
}
#pragma endregion Selectable

