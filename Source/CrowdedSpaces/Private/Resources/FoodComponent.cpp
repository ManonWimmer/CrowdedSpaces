#include "Resources/FoodComponent.h"

UFoodComponent::UFoodComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UFoodComponent::BeginPlay()
{
	Super::BeginPlay();

	if (bSufferHunger)
		StartFoodTimer();
}

void UFoodComponent::AddFood(int Amount)
{
	const int32 OldFood = Food;

	if (bSufferHunger) // NPC
	{
		Food = FMath::Clamp(Food + Amount, 0, 100);
		
		if (Food != OldFood)
		{
			OnStatChanged.Broadcast("Food", FString::SanitizeFloat(Food));
		}

		if (Food >= MaxFood)
		{
			OnFoodFull.Broadcast();
		}
	}
	else // Player controller
	{
		Food += Amount;
		OnStatChanged.Broadcast("Food", FString::SanitizeFloat(Food));
	}
}

void UFoodComponent::RemoveFood(int Amount)
{
	if (!HasEnoughFood(Amount)) return; // todo: plus tard event on no more food et mettre à 0 ? 
	
	Food -= Amount;
	OnStatChanged.Broadcast("Food", FString::SanitizeFloat(Food));
}

bool UFoodComponent::HasEnoughFood(int Amount)
{
	return Food >= Amount;
}

void UFoodComponent::StartFoodTimer()
{
	if (!GetWorld()) return;

	GetWorld()->GetTimerManager().SetTimer(
		FoodTimerHandle,
		this,
		&UFoodComponent::FoodTick,
		TickInterval,
		true
	);
}

void UFoodComponent::StopFoodTimer()
{
	if (!GetWorld()) return;
	GetWorld()->GetTimerManager().ClearTimer(FoodTimerHandle);
}

void UFoodComponent::SetEating(bool bEating)
{
	bIsEating = bEating;
	FString Result = bIsEating ? TEXT("True") : TEXT("False");
	OnStatChanged.Broadcast("Is Eating", Result);
}

void UFoodComponent::FoodTick()
{
	if (bIsEating)
	{
		AddFood(FoodRegenPerTick);
	}
	else
	{
		RemoveFood(FoodLossPerTick);
	}
}

#pragma region Selectable
TArray<TPair<FString, FString>> UFoodComponent::GetCurrentValues() const
{
	TArray<TPair<FString, FString>> Values;
	Values.Emplace(FString("Food"), FString::SanitizeFloat(Food));
	FString Result = bIsEating ? TEXT("True") : TEXT("False");
	Values.Emplace(FString("Is Eating"), Result);
	return Values;
}
#pragma endregion Selectable
