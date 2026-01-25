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
		OnStatChanged.Broadcast("Energy", FString::SanitizeFloat(Energy));
	}

	if (Energy >= MaxEnergy)
	{
		OnEnergyFull.Broadcast();
	}
}

void UEnergyComponent::RemoveEnergy(int Amount)
{
	Energy = FMath::Clamp(Energy - Amount, 0, MaxEnergy);
	OnStatChanged.Broadcast("Energy", FString::SanitizeFloat(Energy));
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
	FString Result = bIsSleeping ? TEXT("True") : TEXT("False");
	OnStatChanged.Broadcast("Is Sleeping", Result);
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

#pragma region Selectable
TArray<TPair<FString, FString>> UEnergyComponent::GetCurrentValues() const
{
	TArray<TPair<FString, FString>> Values;
	Values.Add(TPair<FString, FString>(FString("Energy"), FString::SanitizeFloat(Energy)));
	FString Result = bIsSleeping ? TEXT("True") : TEXT("False");
	Values.Add(TPair<FString, FString>(FString("Is Sleeping"), Result));
	return Values;
}
#pragma endregion Selectable
