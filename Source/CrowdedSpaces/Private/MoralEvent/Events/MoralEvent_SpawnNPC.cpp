#include "MoralEvent/Events/MoralEvent_SpawnNPC.h"

void UMoralEvent_SpawnNPC::StartMoralEvent()
{
	Super::StartMoralEvent();
}

void UMoralEvent_SpawnNPC::ClickOnChoice(int choiceIndex)
{
	Super::ClickOnChoice(choiceIndex);

	
}

void UMoralEvent_SpawnNPC::SetupChoices()
{
	Super::SetupChoices();

	Choices = {TEXT("Spawn NPC"), TEXT("Do nothing")};
}
