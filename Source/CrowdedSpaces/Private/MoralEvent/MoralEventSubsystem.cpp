#include "MoralEvent/MoralEventSubsystem.h"

void UMoralEventSubsystem::StartNewEvent(TSubclassOf<UMoralEvent> NewEvent)
{
	if (!GameHUD)
		GetGameHUD();
	
	CurrentEvent = NewObject<UMoralEvent>(this, NewEvent);
	CurrentEvent->SetupChoices();

	GameHUD->ShowMoralEventWidget(true);
	GameHUD->UpdateMoralEventWidget(CurrentEvent);
}

void UMoralEventSubsystem::HandleRandomMoralEvent()
{
	if (PossibleEvents.Num() == 0)
		return;

	// Get random event in possible events
	int32 RandomIndex = FMath::RandRange(0, PossibleEvents.Num() - 1);
	TSubclassOf<UMoralEvent> RandomEvent = PossibleEvents[RandomIndex];

	// Start event
	StartNewEvent(RandomEvent);
}

void UMoralEventSubsystem::OnChoiceSelected(EMoralEventType Choice)
{
	if (!GameHUD)
		GetGameHUD();

	CurrentEvent->ClickOnChoice(Choice);
	GameHUD->ShowMoralEventWidget(false);
	
	CurrentEvent = nullptr;

	OnMoralEventEnded.Broadcast();
}

void UMoralEventSubsystem::GetGameHUD()
{
	TObjectPtr<APlayerController> PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	GameHUD = Cast<AGameHUD>(PC->GetHUD());
}
