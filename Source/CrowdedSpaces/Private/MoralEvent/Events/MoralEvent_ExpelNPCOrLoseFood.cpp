#include "MoralEvent/Events/MoralEvent_ExpelNPCOrLoseFood.h"

#include "Game/CrowdedGameState.h"

class ACrowdedPlayerController;

void UMoralEvent_ExpelNPCOrLoseFood::ClickOnChoice(EMoralEventType Choice)
{
	Super::ClickOnChoice(Choice);
	
	switch (Choice)
	{
		case EMoralEventType::ExpelNPC:
			{
				KillSelectedNPC();
				break;
			}
		
		case EMoralEventType::LoseFood:
			{
				RemovePlayerResource(EResourceType::Food, FoodToLose);
				break;
			}
		
		default:
			break;
	}
}

void UMoralEvent_ExpelNPCOrLoseFood::SetupChoices()
{
	Super::SetupChoices();

	Choices = {EMoralEventType::ExpelNPC, EMoralEventType::LoseFood};
}
