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
			OnFoodChanged.Broadcast(Food);
		}

		if (Food >= MaxFood)
		{
			OnFoodFull.Broadcast();
		}
	}
	else // Player controller
	{
		Food += Amount;
		OnFoodChanged.Broadcast(Food);
	}
}

void UFoodComponent::RemoveFood(int Amount)
{
	if (!HasEnoughFood(Amount)) return; // todo: plus tard event on no more food et mettre à 0 ? 
	
	Food -= Amount;
	OnFoodChanged.Broadcast(Food);
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
	OnIsEatingChanged.Broadcast(bIsEating);
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
