#include "MoralEvent/Events/MoralEvent_SpawnNPC.h"

void UMoralEvent_SpawnNPC::ClickOnChoice(int ChoiceIndex)
{
	Super::ClickOnChoice(ChoiceIndex);

	FVector Location(0.0f, 0.0f, 140.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	
	switch(ChoiceIndex)
	{
		case 0: // "Spawn NPC"
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, "Decided to do spawn npc.");

			GetWorld()->SpawnActor<AActor>(NPCClass, Location, Rotation, SpawnInfo);
			break;
			
		case 1: // "Do nothing"
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, "Decided to do nothing.");
			break;
		
		default:
			break;
	}
}

void UMoralEvent_SpawnNPC::SetupChoices()
{
	Super::SetupChoices();

	Choices = {TEXT("Spawn NPC"), TEXT("Do nothing")};
}
