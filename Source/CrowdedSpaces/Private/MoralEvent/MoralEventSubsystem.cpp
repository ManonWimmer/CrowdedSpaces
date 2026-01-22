#include "MoralEvent/MoralEventSubsystem.h"

void UMoralEventSubsystem::StartNewEvent(TSubclassOf<UMoralEvent> NewEvent)
{
	if (!GameHUD) GetGameHUD();
	
	CurrentEvent = NewObject<UMoralEvent>(this, NewEvent);
	CurrentEvent->SetupChoices();

	GameHUD->ShowMoralEventWidget(true);
	GameHUD->UpdateMoralEventWidget(CurrentEvent);
}

void UMoralEventSubsystem::OnChoiceSelected(EMoralEventChoice Choice)
{
	if (!GameHUD) GetGameHUD();

	CurrentEvent->ClickOnChoice(Choice);
	GameHUD->ShowMoralEventWidget(false);
	
	CurrentEvent = nullptr;
}

void UMoralEventSubsystem::GetGameHUD()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	GameHUD = Cast<AGameHUD>(PC->GetHUD());
}
