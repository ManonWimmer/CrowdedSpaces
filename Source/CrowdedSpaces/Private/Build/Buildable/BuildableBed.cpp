#include "Build/Buildable/BuildableBed.h"

#include "Build/BuildableRegistrySubsystem.h"

ABuildableBed::ABuildableBed()
{
	// Selectable
	SelectionType = ESelectionType::Bed;
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
	Values.Emplace(FString("Is Available"), Result);
	return Values;
}
#pragma endregion Selectable

bool ABuildableBed::TryReserve(ANPC* NPC)
{
	if (ComingNPC.IsValid())
		return false;

	ComingNPC = NPC;
	
	bHasNPCComing = true;
	OnNPCComingToBedChanged.Broadcast(bHasNPCComing);
	
	return true;
}

bool ABuildableBed::IsReservedByOther(TObjectPtr<ANPC> NPC)
{
	return ComingNPC.IsValid() && ComingNPC != NPC;
}

void ABuildableBed::Release(ANPC* NPC)
{
	if (ComingNPC != NPC)
		return;
	
	ComingNPC = nullptr;
	
	bHasNPCComing = false;
	OnNPCComingToBedChanged.Broadcast(bHasNPCComing);
}

void ABuildableBed::StartSleeping(ANPC* NPC)
{
	if (ComingNPC != NPC)
		return;
	
	ComingNPC = nullptr;
	EatingNPC = NPC;
	
	bHasNPCComing = false;
	OnNPCComingToBedChanged.Broadcast(bHasNPCComing);

	bHasNPCSleeping = true;
	OnNPCSleepingChanged.Broadcast(bHasNPCComing);
}

void ABuildableBed::StopSleeping(ANPC* NPC)
{
	if (EatingNPC != NPC)
		return;
	
	EatingNPC = nullptr;
	
	bHasNPCSleeping = false;
	OnNPCSleepingChanged.Broadcast(bHasNPCComing);
}


