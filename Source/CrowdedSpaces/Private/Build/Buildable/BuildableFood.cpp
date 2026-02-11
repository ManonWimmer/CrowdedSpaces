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
	//FString Result = bIsAvailable ? TEXT("True") : TEXT("False");
	//Values.Emplace(FString("Is Available"), Result);
	return Values;
}
#pragma endregion Selectable

bool ABuildableFood::TryReserve(ANPC* NPC)
{
	if (ComingNPC.IsValid())
		return false;

	ComingNPC = NPC;
	
	bHasNPCComing = true;
	OnNPCComingToFoodChanged.Broadcast(bHasNPCComing);
	
	return true;
}

bool ABuildableFood::IsReservedByOther(TObjectPtr<ANPC> NPC)
{
	return ComingNPC.IsValid() && ComingNPC != NPC;
}

void ABuildableFood::Release(ANPC* NPC)
{
	if (ComingNPC != NPC)
		return;
	
	ComingNPC = nullptr;
	
	bHasNPCComing = false;
	OnNPCComingToFoodChanged.Broadcast(bHasNPCComing);
}

void ABuildableFood::StartEating(ANPC* NPC)
{
	if (ComingNPC != NPC)
		return;
	
	ComingNPC = nullptr;
	EatingNPC = NPC;
	
	bHasNPCComing = false;
	OnNPCComingToFoodChanged.Broadcast(bHasNPCComing);

	bHasNPCEating = true;
	OnNPCEatingChanged.Broadcast(bHasNPCComing);
}

void ABuildableFood::StopEating(ANPC* NPC)
{
	if (EatingNPC != NPC)
		return;
	
	EatingNPC = nullptr;
	
	bHasNPCEating = false;
	OnNPCEatingChanged.Broadcast(bHasNPCComing);
}


