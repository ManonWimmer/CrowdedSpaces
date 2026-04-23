#include "MoralEvent/Events/MoralEvent_Fire.h"

void UMoralEvent_Fire::ClickOnChoice(EMoralEventType Choice)
{
	Super::ClickOnChoice(Choice);
	
	if (Choice != EMoralEventType::Accept)
		return;
	
	// Start fire
}

void UMoralEvent_Fire::SetupChoices()
{
	Super::SetupChoices();

	Choices = {EMoralEventType::Accept};
}
