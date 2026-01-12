#include "Resources/FoodComponent.h"

UFoodComponent::UFoodComponent()
{
}

void UFoodComponent::AddFood(int Amount)
{
	Food += Amount;
	OnStatChanged.Broadcast("Food", Food);
}

void UFoodComponent::RemoveFood(int Amount)
{
	if (!HasEnoughFood(Amount)) return; // todo: plus tard event on no more food et mettre à 0 ? 
	
	Food -= Amount;
	OnStatChanged.Broadcast("Food", Food);
}

bool UFoodComponent::HasEnoughFood(int Amount)
{
	return Food >= Amount;
}
