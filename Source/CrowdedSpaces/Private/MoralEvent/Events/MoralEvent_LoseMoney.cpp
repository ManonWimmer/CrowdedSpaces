#include "MoralEvent/Events/MoralEvent_LoseMoney.h"

#include "Resources/ResourceType.h"

void UMoralEvent_LoseMoney::ClickOnChoice(EMoralEventType Choice)
{
	Super::ClickOnChoice(Choice);

	if (Choice != EMoralEventType::Accept)
		return;

	RemovePlayerResource(EResourceType::Money, MoneyToLose);
}

void UMoralEvent_LoseMoney::SetupChoices()
{
	Super::SetupChoices();

	Choices = {EMoralEventType::Accept};
}
