#include "MoralEvent/MoralEventSubsystem.h"

#include "AI/NPC.h"

void UMoralEventSubsystem::StartNewEvent(UMoralEventData* EventData)
{
	if (!GameHUD) GetGameHUD();
	
	CurrentEventData = EventData;

	GameHUD->ShowMoralEventWidget(true);
	GameHUD->UpdateMoralEventWidget(CurrentEventData);
}

void UMoralEventSubsystem::EndCurrentEvent(bool bSelected)
{
	if (!GameHUD) GetGameHUD();

	GameHUD->ShowMoralEventWidget(false);

	if (bSelected)
	{
		switch(CurrentEventData->Type)
		{
		case EMoralEventType::NewNPC:
				if(GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, "Spawn New NPC");
				
				FVector Location(0.0f, 0.0f, 140.0f);
				FRotator Rotation(0.0f, 0.0f, 0.0f);
				FActorSpawnParameters SpawnInfo;
				
				GetWorld()->SpawnActor<AActor>(CurrentEventData->NPCClass, Location, Rotation, SpawnInfo);
				break;
		}
	}
	
	
	CurrentEventData = nullptr;
}

// Plus tard : end current event(choice index) -> current moral event on choice selected

void UMoralEventSubsystem::GetGameHUD()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	GameHUD = Cast<AGameHUD>(PC->GetHUD());
}
