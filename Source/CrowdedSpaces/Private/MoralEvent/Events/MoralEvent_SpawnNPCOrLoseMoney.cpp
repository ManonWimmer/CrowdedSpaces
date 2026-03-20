#include "MoralEvent/Events/MoralEvent_SpawnNPCOrLoseMoney.h"

#include "AI/NPCFunctionLibrary.h"

void UMoralEvent_SpawnNPCOrLoseMoney::ClickOnChoice(EMoralEventType Choice)
{
	Super::ClickOnChoice(Choice);
	
	switch (Choice)
	{
		case EMoralEventType::SpawnNPC:
			{
				ANPC* SpawnedNPC = UNPCFunctionLibrary::SpawnGlobalNPC(this);
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
