#include "Resources/FoodComponent.h"

UFoodComponent::UFoodComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
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

#pragma region Selectable
TArray<TPair<FString, FString>> UFoodComponent::GetCurrentValues() const
{
	TArray<TPair<FString, FString>> Values;
	Values.Add(TPair<FString, FString>(FString("Food"), FString::SanitizeFloat(Food)));
	Values.Add(TPair<FString, FString>(FString("Test"), FString::SanitizeFloat(1000)));
	return Values;
}
#pragma endregion Selectable