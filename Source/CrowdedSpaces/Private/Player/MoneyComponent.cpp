// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MoneyComponent.h"


// Sets default values for this component's properties
UMoneyComponent::UMoneyComponent()
{
}

void UMoneyComponent::AddMoney(int Amount)
{
	Money += Amount;
	OnMoneyChanged.Broadcast(Money);
}

void UMoneyComponent::RemoveMoney(int Amount)
{
	if (!HasEnoughMoney(Amount)) return;
	
	Money -= Amount;
	OnMoneyChanged.Broadcast(Money);
}

bool UMoneyComponent::HasEnoughMoney(int Amount)
{
	return Money >= Amount;
}
