#include "MoralEvent/Events/MoralEvent_RandomNPCDeath.h"

#include "EngineUtils.h"

void UMoralEvent_RandomNPCDeath::ClickOnChoice(EMoralEventType Choice)
{
	Super::ClickOnChoice(Choice);

	if (Choice != EMoralEventType::Accept)
		return;

	// pas de switch vu que seulement choix d'accept
	
	if (IsValid(SelectedNPC))
	{
		SelectedNPC->Die();
	}
}

void UMoralEvent_RandomNPCDeath::SetupChoices()
{
	Super::SetupChoices();

	Choices = {EMoralEventType::Accept};

	const TObjectPtr<UWorld> World = GetWorld();
	if (!World)
		return;

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

	// Change description with npc name
	if (SelectedNPC)
	{
		const FString NPCName = SelectedNPC->GetNPCName(); 
		
		EventDescription = EventDescription.Replace(TEXT("{npc}"), *NPCName);
	}
}
