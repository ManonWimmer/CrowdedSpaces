#include "MoralEvent/Events/MoralEvent_Fire.h"

#include "Fire/FireSubsystem.h"

void UMoralEvent_Fire::ClickOnChoice(EMoralEventType Choice)
{
	Super::ClickOnChoice(Choice);
	
	if (Choice != EMoralEventType::Accept)
		return;
	
	const TObjectPtr<UFireSubsystem> FireSubsystem = GetWorld()->GetSubsystem<UFireSubsystem>();
	if (!FireSubsystem)
		return;

	FireSubsystem->StartFire();
}

void UMoralEvent_Fire::SetupChoices()
{
	Super::SetupChoices();

	Choices = {EMoralEventType::Accept};
}
