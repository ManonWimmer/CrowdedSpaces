#include "Time/TimeSubsystem.h"

TStatId UTimeSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UTimeSubsystem, STATGROUP_Tickables);
}

void UTimeSubsystem::Tick(float DeltaTime)
{
	if (!TimeData)
		return;

	float GameSpeedMultiplier = 1; // 1 : 1sec IRL = 1 minute in-game
	CurrentMinutes = FMath::Fmod((CurrentMinutes + GameSpeedMultiplier * DeltaTime),1440); // Modulo pour 1440 minutes pas jours
	
	OnTimeChanged.Broadcast(CurrentMinutes);
}

bool UTimeSubsystem::IsTickable() const
{
	return true;
}
