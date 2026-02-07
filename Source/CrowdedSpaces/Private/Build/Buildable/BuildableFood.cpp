#include "Build/Buildable/BuildableFood.h"

#include "Build/BuildableRegistrySubsystem.h"

ABuildableFood::ABuildableFood()
{
	// Selectable
	SelectionType = ESelectionType::Food;
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
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Set food available");
	
	bIsAvailable = NewAvailable;
	FString Result = bIsAvailable ? TEXT("True") : TEXT("False");
	OnStatChanged.Broadcast("Is Available", Result);
	OnIsFoodAvailableChanged.Broadcast(bIsAvailable);

	if (!bIsAvailable)
		SetHasNPCComing(false);
}

void ABuildableFood::SetHasNPCComing(bool NewAvailable)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Set npc coming to food");
	bHasNPCComing = NewAvailable;
	OnNPCComingToFoodChanged.Broadcast(bHasNPCComing);
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

TObjectPtr<AActor> ABuildableFood::GetSelectableActor()
{
	return this;
}

TArray<FStat> ABuildableFood::GetCurrentValues() const
{
	TArray<TPair<FString, FString>> Values;
	FString Result = bIsAvailable ? TEXT("True") : TEXT("False");
	Values.Emplace(FString("Is Available"), Result);
	return Values;
}
#pragma endregion Selectable

