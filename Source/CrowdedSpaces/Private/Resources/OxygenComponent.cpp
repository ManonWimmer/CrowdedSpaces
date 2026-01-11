#include "Resources/OxygenComponent.h"

UOxygenComponent::UOxygenComponent()
{
}

void UOxygenComponent::AddOxygen(int Amount)
{
	Oxygen += Amount;
	OnOxygenChanged.Broadcast(Oxygen);
}

void UOxygenComponent::RemoveOxygen(int Amount)
{
	if (!HasEnoughOxygen(Amount)) return;
	
	Oxygen -= Amount;
	OnOxygenChanged.Broadcast(Oxygen);
}

bool UOxygenComponent::HasEnoughOxygen(int Amount)
{
	return Oxygen >= Amount;
}


