#include "MoralEvent/Events/MoralEvent_SpawnNPC.h"

#include "AI/NPCHelpers.h"

void UMoralEvent_SpawnNPC::ClickOnChoice(EMoralEventType Choice)
{
	Super::ClickOnChoice(Choice);

	if (Choice != EMoralEventType::Accept)
		return;
	
	ANPC* SpawnedNPC = NPCHelpers::SpawnGlobalNPC(*GetWorld());
}

void UMoralEvent_SpawnNPC::SetupChoices()
{
	Super::SetupChoices();

	Choices = {EMoralEventType::Accept};
}
