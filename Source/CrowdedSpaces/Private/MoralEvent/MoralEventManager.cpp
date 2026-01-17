#include "MoralEvent/MoralEventManager.h"

#include "AI/NPC.h"

void UMoralEventManager::StartNewEvent(UMoralEventData* EventData)
{
	if (!GameHUD) GetGameHUD();
	
	CurrentEventData = EventData;

	GameHUD->ShowMoralEventWidget(true);
	GameHUD->UpdateMoralEventWidget(CurrentEventData);
}

void UMoralEventManager::EndCurrentEvent(bool bSelected)
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

void UMoralEventManager::GetGameHUD()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	GameHUD = Cast<AGameHUD>(PC->GetHUD());
}
