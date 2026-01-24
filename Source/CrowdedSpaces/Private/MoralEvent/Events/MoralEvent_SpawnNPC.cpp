#include "MoralEvent/Events/MoralEvent_SpawnNPC.h"

void UMoralEvent_SpawnNPC::ClickOnChoice(EMoralEventChoice Choice)
{
	Super::ClickOnChoice(Choice);

	FVector Location(0.0f, 0.0f, 140.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	
	switch (Choice)
	{
		case EMoralEventChoice::SpawnNPC: 
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, "Decided to do spawn npc.");

			GetWorld()->SpawnActor<AActor>(NPCClass, Location, Rotation, SpawnInfo);
			break;
			
		case EMoralEventChoice::DoNothing: 
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, "Decided to do nothing.");
			break;
		
		default:
			break;
	}
}

void UMoralEvent_SpawnNPC::SetupChoices()
{
	Super::SetupChoices();

	Choices = {EMoralEventChoice::SpawnNPC, EMoralEventChoice::DoNothing};
}
