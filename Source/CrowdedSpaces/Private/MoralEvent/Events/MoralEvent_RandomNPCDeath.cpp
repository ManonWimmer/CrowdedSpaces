#include "MoralEvent/Events/MoralEvent_RandomNPCDeath.h"

void UMoralEvent_RandomNPCDeath::ClickOnChoice(EMoralEventType Choice)
{
	Super::ClickOnChoice(Choice);

	if (Choice != EMoralEventType::Accept)
		return;
	
	KillSelectedNPC();
}

void UMoralEvent_RandomNPCDeath::SetupChoices()
{
	Super::SetupChoices();

	Choices = {EMoralEventType::Accept};
}