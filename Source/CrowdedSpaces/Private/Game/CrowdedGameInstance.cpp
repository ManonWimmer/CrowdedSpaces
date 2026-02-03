#include "Game/CrowdedGameInstance.h"

#include "MoralEvent/MoralEventSubsystem.h"
#include "Time/TimeSubsystem.h"

void UCrowdedGameInstance::Init()
{
	Super::Init();

	FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &UCrowdedGameInstance::OnPostWorldInitialization);
}

void UCrowdedGameInstance::OnPostWorldInitialization(UWorld* World, const UWorld::InitializationValues IVS)
{
	if (!World)
		return;

	TObjectPtr<UTimeSubsystem> TimeSubsystem = World->GetSubsystem<UTimeSubsystem>();
	if (!TimeSubsystem)
		return;
	
	TObjectPtr<UMoralEventSubsystem> MoralSubsystem = World->GetSubsystem<UMoralEventSubsystem>();
	if (!MoralSubsystem)
		return;

	// Link time & moral subsystems :
	
	// On start moral event
	TimeSubsystem->OnMoralEventTime.AddDynamic(
		MoralSubsystem,
		&UMoralEventSubsystem::HandleRandomMoralEvent
	);

	TimeSubsystem->OnMoralEventTime.AddDynamic(
		TimeSubsystem,
		&UTimeSubsystem::SetTimePaused
	);

	// On end moral event
	MoralSubsystem->OnMoralEventEnded.AddDynamic(
		TimeSubsystem,
		&UTimeSubsystem::SetTimeNormal
	);
}
