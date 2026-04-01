#include "MoralEvent/Events/MoralEvent_SpawnNPCOrLoseMoney.h"

#include "AI/NPCHelpers.h"

void UMoralEvent_SpawnNPCOrLoseMoney::ClickOnChoice(EMoralEventType Choice)
{
	Super::ClickOnChoice(Choice);
	
	switch (Choice)
	{
		case EMoralEventType::SpawnNPC:
			{
				ANPC* SpawnedNPC = NPCHelpers::SpawnGlobalNPC(*GetWorld());
				break;
			}
	
		case EMoralEventType::LoseMoney:
			{
				RemovePlayerResource(EResourceType::Money, MoneyToLose);
				break;
			}
	
		default:
			break;
	}
}

void UMoralEvent_SpawnNPCOrLoseMoney::SetupChoices()
{
	Super::SetupChoices();

	Choices = {EMoralEventType::SpawnNPC, EMoralEventType::LoseMoney};
}
