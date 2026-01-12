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


