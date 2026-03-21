#include "Game/CrowdedGameInstance.h"

#include "Game/CrowdedGameMode.h"
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

	TimeSubsystem = World->GetSubsystem<UTimeSubsystem>();
	if (!TimeSubsystem)
		return;

	const TObjectPtr<UMoralEventSubsystem> MoralSubsystem = World->GetSubsystem<UMoralEventSubsystem>();
	if (!MoralSubsystem)
		return;

	// Link time & moral subsystems :
	
	// On start moral event
	TimeSubsystem->OnMoralEventTime.AddDynamic(
		MoralSubsystem,
		&UMoralEventSubsystem::HandleCurrentDayMoralEvent
	);

	TimeSubsystem->OnMoralEventTime.AddDynamic(
		TimeSubsystem,
		&UTimeSubsystem::SetTimePausedWithEvent
	);

	// On end moral event
	MoralSubsystem->OnMoralEventEnded.AddDynamic(
		TimeSubsystem,
		&UTimeSubsystem::SetTimeUnpaused
	);
}

void UCrowdedGameInstance::ResetGameSettings()
{
	bNPCsCanLoseFood = true;
}
