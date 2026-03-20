#include "MoralEvent/MoralEvent.h"

#include "EngineUtils.h"
#include "AI/NPC.h"
#include "Game/CrowdedGameState.h"

void UMoralEvent::ClickOnChoice(EMoralEventType Choice)
{
}

void UMoralEvent::SetupChoices()
{
	const UWorld* World = GetWorld();
	if (!World)
		return;

	// Player & resource components
	GameState = World->GetGameState<ACrowdedGameState>();
	if (!GameState)
		return;

	// Select random npc
	TArray<TObjectPtr<ANPC>> FoundNPCs;

	for (TActorIterator<ANPC> It(World); It; ++It)
	{
		TObjectPtr<ANPC> NPC = *It;
		if (!IsValid(NPC))
			continue;

		FoundNPCs.Add(NPC);
	}

	if (FoundNPCs.IsEmpty())
		return;

	const int32 RandomIndex = FMath::RandRange(0, FoundNPCs.Num() - 1);
	SelectedNPC = FoundNPCs[RandomIndex];

	// Change title & description with selected npc name
	if (!SelectedNPC)
		return;
	
	const FString NPCName = SelectedNPC->GetNPCName();
	EventTitle = EventTitle.Replace(TEXT("{npc}"), *NPCName);
	EventDescription = EventDescription.Replace(TEXT("{npc}"), *NPCName);
}

AActor* UMoralEvent::SpawnNPC(const TSubclassOf<ANPC> NPCClass) const
{
	const FVector Location(0.0f, 0.0f, 140.0f);
	const FRotator Rotation(0.0f, 0.0f, 0.0f);

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	return GetWorld()->SpawnActor<ANPC>(NPCClass, Location, Rotation, SpawnInfo);
}

void UMoralEvent::KillSelectedNPC() const
{
	if (!SelectedNPC)
		return;

	SelectedNPC->Die();
}

void UMoralEvent::AddPlayerResource(const EResourceType ResourceType, const int Amount) const
{
	if (!GameState)
		return;

	UResourceComponent* ResourceComponent = GameState->GetResourceComponentByType(ResourceType);
	if (!ResourceComponent)
		return;
	
	ResourceComponent->AddResource(Amount);
}

void UMoralEvent::RemovePlayerResource(const EResourceType ResourceType, const int Amount) const
{
	if (!GameState)
		return;

	UResourceComponent* ResourceComponent = GameState->GetResourceComponentByType(ResourceType);
	if (!ResourceComponent)
		return;
	
	ResourceComponent->RemoveResource(Amount);
}
