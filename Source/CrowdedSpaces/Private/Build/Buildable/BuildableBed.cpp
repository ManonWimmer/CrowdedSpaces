#include "Build/Buildable/BuildableBed.h"

#include "Build/BuildableRegistrySubsystem.h"

ABuildableBed::ABuildableBed()
{
}

void ABuildableBed::BeginPlay()
{
	Super::BeginPlay();

	TObjectPtr<UBuildableRegistrySubsystem> BRS = GetWorld()->GetSubsystem<UBuildableRegistrySubsystem>();
	if (!BRS)
		return;

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Register bed");

	BRS->RegisterBed(this);
}

void ABuildableBed::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	TObjectPtr<UBuildableRegistrySubsystem> BRS = GetWorld()->GetSubsystem<UBuildableRegistrySubsystem>();
	if (!BRS)
		return;

	BRS->UnregisterBed(this);
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

TObjectPtr<AActor> ABuildableBed::GetSelectableActor()
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

