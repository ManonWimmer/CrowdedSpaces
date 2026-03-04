#include "MoralEvent/Events/MoralEvent_SpawnNPC.h"

void UMoralEvent_SpawnNPC::ClickOnChoice(EMoralEventType Choice)
{
	Super::ClickOnChoice(Choice);

	if (Choice != EMoralEventType::Accept)
		return;
	
	AActor* NPC = SpawnNPC(NPCClass);
}

void UMoralEvent_SpawnNPC::SetupChoices()
{
	Super::SetupChoices();

	Choices = {EMoralEventType::Accept};
}
