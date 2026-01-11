#include "Resources/ElectricityComponent.h"

UElectricityComponent::UElectricityComponent()
{
}

void UElectricityComponent::AddElectricity(int Amount)
{
	Electricity += Amount;
	OnElectricityChanged.Broadcast(Electricity);
}

void UElectricityComponent::RemoveElectricity(int Amount)
{
	if (!HasEnoughElectricity(Amount)) return;
	
	Electricity -= Amount;
	OnElectricityChanged.Broadcast(Electricity);
}

bool UElectricityComponent::HasEnoughElectricity(int Amount)
{
	return Electricity >= Amount;
}

