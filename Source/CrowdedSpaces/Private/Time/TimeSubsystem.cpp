#include "Time/TimeSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "Time/TimeSpeedType.h"

TStatId UTimeSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UTimeSubsystem, STATGROUP_Tickables);
}

void UTimeSubsystem::Tick(float DeltaTime)
{
	if (!TimeData)
		return;

	TotalMinutes += CurrentSpeedMultiplier * DeltaTime;

	// Current day
	int NewDay = (TotalMinutes / 1440) + 1;
	if (NewDay != CurrentDay)
	{
		CurrentDay = NewDay;
		OnDayChanged.Broadcast(CurrentDay);
	}

	// Current minutes
	CurrentMinutes = FMath::Fmod(TotalMinutes, 1440); // Modulo pour 1440 minutes pas jours
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

void UTimeSubsystem::SetGameSpeed(ETimeSpeedType NewTimeSpeed)
{
	CurrentTimeSpeed = NewTimeSpeed;
	GetCurrentSpeedValues();
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), CurrentSpeedTimeDilation);
}

void UTimeSubsystem::GetCurrentSpeedValues()
{
	for (FTimeDataStruct SpeedData : TimeData->TimeData)
	{
		if (SpeedData.TimeSpeedType == CurrentTimeSpeed)
		{
			CurrentSpeedMultiplier = SpeedData.GameSpeedMultiplier;
			CurrentSpeedTimeDilation = SpeedData.GameTimeDilation;
			return;
		}
	}
}
