#include "Resources/EnergyComponent.h"

UEnergyComponent::UEnergyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEnergyComponent::BeginPlay()
{
	Super::BeginPlay();
	StartEnergyTimer();
}

void UEnergyComponent::AddEnergy(int Amount)
{
	const int32 OldEnergy = Energy;

	Energy = FMath::Clamp(Energy + Amount, 0, MaxEnergy);

	if (Energy != OldEnergy)
	{
		OnEnergyChanged.Broadcast(Energy);
	}

	if (Energy >= MaxEnergy)
	{
		OnEnergyFull.Broadcast();
	}
}

void UEnergyComponent::RemoveEnergy(int Amount)
{
	Energy = FMath::Clamp(Energy - Amount, 0, MaxEnergy);
	
	if (Energy <= 0)
		OnNoMoreEnergy.Broadcast();
	else
		OnEnergyChanged.Broadcast(Energy);
}

bool UEnergyComponent::HasEnoughEnergy(int Amount)
{
	return Energy >= Amount;
}

bool UEnergyComponent::HasMaxEnergy()
{
	return Energy >= MaxEnergy;
}

void UEnergyComponent::StartEnergyTimer()
{
	if (!GetWorld()) return;

	GetWorld()->GetTimerManager().SetTimer(
		EnergyTimerHandle,
		this,
		&UEnergyComponent::EnergyTick,
		TickInterval,
		true
	);
}

void UEnergyComponent::StopEnergyTimer()
{
	if (!GetWorld()) return;
	GetWorld()->GetTimerManager().ClearTimer(EnergyTimerHandle);
}

void UEnergyComponent::SetSleeping(bool bSleeping)
{
	bIsSleeping = bSleeping;
	OnIsSleepingChanged.Broadcast(bIsSleeping);
}

void UEnergyComponent::EnergyTick()
{
	if (bIsSleeping)
	{
		AddEnergy(EnergyRegenPerTick);
	}
	else
	{
		RemoveEnergy(EnergyLossPerTick);
	}
}