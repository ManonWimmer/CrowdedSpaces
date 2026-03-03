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

void UMoralEventSubsystem::HandleCurrentDayMoralEvent(const TSubclassOf<UMoralEvent> MoralEvent)
{
	// Start event
	StartNewEvent(MoralEvent);
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
