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
TArray<TPair<FString, float>> UElectricityComponent::GetCurrentValues() const
{
	TArray<TPair<FString, float>> values;
	values.Add(TPair<FString, float>(FString("Electricity"), Electricity));
	return values;
}
#pragma endregion Selectable

