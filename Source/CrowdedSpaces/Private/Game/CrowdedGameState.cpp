#include "Game/CrowdedGameState.h"

#include "Build/BuildSubsystem.h"
#include "MoralEvent/MoralEventSubsystem.h"
#include "Time/TimeSubsystem.h"

void ACrowdedGameState::BeginPlay()
{
	Super::BeginPlay();

	// Get build subsystem & send data
	TObjectPtr<UBuildSubsystem> BuildSubsystem = GetWorld()->GetSubsystem<UBuildSubsystem>();
	if (!BuildSubsystem)
		return;
		
	BuildSubsystem->SetBuildData(BuildDataObjects);
	BuildSubsystem->SetBuildRoomData(BuildDataRooms);
	BuildSubsystem->SetSnapSize(SnapSize);

	// Get time subsystem & send data
	TObjectPtr<UTimeSubsystem> TimeSubsystem = GetWorld()->GetSubsystem<UTimeSubsystem>();
	if (!TimeSubsystem)
		return;

	TimeSubsystem->SetTimeData(TimeData);

	// Get moral event subsystem & send data
	TObjectPtr<UMoralEventSubsystem> MoralEventSubsystem = GetWorld()->GetSubsystem<UMoralEventSubsystem>();
	if (!MoralEventSubsystem)
		return;

	MoralEventSubsystem->SetPossibleEvents(PossibleMoralEvents);
}
