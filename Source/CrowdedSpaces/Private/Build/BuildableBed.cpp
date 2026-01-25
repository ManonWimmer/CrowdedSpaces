#include "Build/BuildableBed.h"

ABuildableBed::ABuildableBed()
{
}

void ABuildableBed::BeginPlay()
{
	Super::BeginPlay();
}


void ABuildableBed::SetAvailable(bool NewAvailable)
{
	bIsAvailable = NewAvailable;
	FString Result = bIsAvailable ? TEXT("True") : TEXT("False");
	OnStatChanged.Broadcast("Is Available", Result);
}

#pragma region Selectable
void ABuildableBed::OnSelected()
{
}

void ABuildableBed::OnDeselected()
{
}

FString ABuildableBed::GetDisplayName() const
{
	return "Bed";
}

AActor* ABuildableBed::GetSelectableActor()
{
	return this;
}

TArray<FStat> ABuildableBed::GetCurrentValues() const
{
	TArray<TPair<FString, FString>> Values;
	FString Result = bIsAvailable ? TEXT("True") : TEXT("False");
	Values.Add(TPair<FString, FString>(FString("Is Available"), Result));
	return Values;
}
#pragma endregion Selectable

