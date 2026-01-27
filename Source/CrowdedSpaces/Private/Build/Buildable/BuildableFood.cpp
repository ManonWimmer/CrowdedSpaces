#include "Build/Buildable/BuildableFood.h"

ABuildableFood::ABuildableFood()
{
}

void ABuildableFood::BeginPlay()
{
	Super::BeginPlay();
}

void ABuildableFood::SetAvailable(bool NewAvailable)
{
	bIsAvailable = NewAvailable;
	FString Result = bIsAvailable ? TEXT("True") : TEXT("False");
	OnStatChanged.Broadcast("Is Available", Result);
}

#pragma region Selectable
void ABuildableFood::OnSelected()
{
}

void ABuildableFood::OnDeselected()
{
}

FString ABuildableFood::GetDisplayName() const
{
	return "Food";
}

AActor* ABuildableFood::GetSelectableActor()
{
	return this;
}

TArray<FStat> ABuildableFood::GetCurrentValues() const
{
	TArray<TPair<FString, FString>> Values;
	FString Result = bIsAvailable ? TEXT("True") : TEXT("False");
	Values.Add(TPair<FString, FString>(FString("Is Available"), Result));
	return Values;
}
#pragma endregion Selectable

