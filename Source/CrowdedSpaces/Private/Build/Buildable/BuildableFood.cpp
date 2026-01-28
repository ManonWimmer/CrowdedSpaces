#include "Build/Buildable/BuildableFood.h"

#include "Build/BuildableRegistrySubsystem.h"

ABuildableFood::ABuildableFood()
{
}

void ABuildableFood::BeginPlay()
{
	Super::BeginPlay();

	UBuildableRegistrySubsystem* BRS = GetWorld()->GetSubsystem<UBuildableRegistrySubsystem>();
	if (!BRS)
		return;

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Register food");

	BRS->RegisterFood(this);
}

void ABuildableFood::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UBuildableRegistrySubsystem* BRS = GetWorld()->GetSubsystem<UBuildableRegistrySubsystem>();
	if (!BRS)
		return;
	
	BRS->UnregisterFood(this);
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

