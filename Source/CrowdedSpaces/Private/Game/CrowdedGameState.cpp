#include "Game/CrowdedGameState.h"

#include "Build/BuildSubsystem.h"
#include "MoralEvent/MoralEventSubsystem.h"
#include "Time/TimeSubsystem.h"

void ACrowdedGameState::BeginPlay()
{
	Super::BeginPlay();

	// Get build subsystem & send data
	UBuildSubsystem* BuildSubsystem = GetWorld()->GetSubsystem<UBuildSubsystem>();
	if (!BuildSubsystem)
		return;
		
	BuildSubsystem->SetBuildData(BuildDataObjects);
	BuildSubsystem->SetSnapSize(SnapSize);

	// Get time subsystem & send data
	UTimeSubsystem* TimeSubsystem = GetWorld()->GetSubsystem<UTimeSubsystem>();
	if (!TimeSubsystem)
		return;

	TimeSubsystem->SetTimeData(TimeData);

	// Get moral event subsystem & send data
	UMoralEventSubsystem* MoralEventSubsystem = GetWorld()->GetSubsystem<UMoralEventSubsystem>();
	if (!MoralEventSubsystem)
		return;

	MoralEventSubsystem->SetPossibleEvents(PossibleMoralEvents);
}
