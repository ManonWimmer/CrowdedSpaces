#include "Game/CrowdedGameInstance.h"

#include "Electricity/ElectricitySubsystem.h"
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

	// Link time & moral subsystems :
	const TObjectPtr<UMoralEventSubsystem> MoralSubsystem = World->GetSubsystem<UMoralEventSubsystem>();
	if (!MoralSubsystem)
		return;

	TimeSubsystem->OnMoralEventTime.AddDynamic(MoralSubsystem, &UMoralEventSubsystem::HandleCurrentDayMoralEvent);
	TimeSubsystem->OnMoralEventTime.AddDynamic(TimeSubsystem, &UTimeSubsystem::SetTimePausedWithEvent);
	MoralSubsystem->OnMoralEventEnded.AddDynamic(TimeSubsystem, &UTimeSubsystem::SetTimeUnpaused);

	// Link time & electricity subsystem :
	const TObjectPtr<UElectricitySubsystem> ElectricitySubsystem = World->GetSubsystem<UElectricitySubsystem>();
	if (!ElectricitySubsystem)
		return;

	TimeSubsystem->OnTimeChanged.AddDynamic(ElectricitySubsystem, &UElectricitySubsystem::OnTimeChanged);
}

void UCrowdedGameInstance::ResetGameSettings()
{
	bNPCsCanLoseFood = true;
}
