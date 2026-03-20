#include "MoralEvent/Events/MoralEvent_SpawnNPC.h"

#include "AI/NPCFunctionLibrary.h"

void UMoralEvent_SpawnNPC::ClickOnChoice(EMoralEventType Choice)
{
	Super::ClickOnChoice(Choice);

	if (Choice != EMoralEventType::Accept)
		return;
	
	ANPC* SpawnedNPC = UNPCFunctionLibrary::SpawnGlobalNPC(this);
}

void UMoralEvent_SpawnNPC::SetupChoices()
{
	Super::SetupChoices();

	Choices = {EMoralEventType::Accept};
}
