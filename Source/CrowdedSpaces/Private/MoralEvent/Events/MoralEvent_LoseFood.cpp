#include "MoralEvent/Events/MoralEvent_LoseFood.h"

#include "Resources/ResourceType.h"

void UMoralEvent_LoseFood::ClickOnChoice(EMoralEventType Choice)
{
	Super::ClickOnChoice(Choice);

	if (Choice != EMoralEventType::Accept)
		return;

	RemovePlayerResource(EResourceType::Food, FoodToLose);
}

void UMoralEvent_LoseFood::SetupChoices()
{
	Super::SetupChoices();

	Choices = {EMoralEventType::Accept};
}
