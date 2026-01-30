#include "Time/TimeSubsystem.h"

TStatId UTimeSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UTimeSubsystem, STATGROUP_Tickables);
}

void UTimeSubsystem::Tick(float DeltaTime)
{
	if (!TimeData)
		return;
	
	CurrentMinutes = FMath::Fmod((CurrentMinutes + CurrentSpeedMultiplier * DeltaTime),1440); // Modulo pour 1440 minutes pas jours
	
	OnTimeChanged.Broadcast(CurrentMinutes);
}

bool UTimeSubsystem::IsTickable() const
{
	return true;
}

void UTimeSubsystem::SetTimeData(UTimeData* NewTimeData)
{
	TimeData = NewTimeData;
	GetCurrentSpeedValues();
}

void UTimeSubsystem::SetGameSpeed(EGameSpeedType NewGameSpeed)
{
	CurrentGameSpeed = NewGameSpeed;
	GetCurrentSpeedValues();
}

void UTimeSubsystem::GetCurrentSpeedValues()
{
	for (FTimeDataStruct SpeedData : TimeData->TimeData)
	{
		if (SpeedData.GameSpeedType == CurrentGameSpeed)
		{
			CurrentSpeedMultiplier = SpeedData.GameSpeedMultiplier;
			CurrentSpeedTimeDilation = SpeedData.GameTimeDilation;
			return;
		}
	}
}
