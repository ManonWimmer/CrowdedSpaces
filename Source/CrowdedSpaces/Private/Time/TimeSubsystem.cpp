#include "Time/TimeSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "Player/CrowdedPlayerController.h"
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
	const int NewDay = (TotalMinutes / 1440) + 1;
	if (NewDay != CurrentDay)
	{
		CurrentDay = NewDay;
		OnDayChanged.Broadcast(CurrentDay);
	}

	// Current minutes
	CurrentMinutes = FMath::Fmod(TotalMinutes, 1440); // Modulo pour 1440 minutes pas jours
	OnTimeChanged.Broadcast(CurrentMinutes);

	if (CurrentMinutes > TimeData->MoralEventHour * 60 && LastDayMoralEvent != CurrentDay)
	{
		LastDayMoralEvent = CurrentDay;
		
		GetRandomMoralEventForDay(CurrentDay);
	}
}

bool UTimeSubsystem::IsTickable() const
{
	return true;
}

void UTimeSubsystem::OnTime0()
{
	TrySetTimeSpeed(ETimeSpeedType::Paused);
}

void UTimeSubsystem::OnTime1()
{
	TrySetTimeSpeed(ETimeSpeedType::Normal);
}

void UTimeSubsystem::OnTime2()
{
	TrySetTimeSpeed(ETimeSpeedType::High);
}

void UTimeSubsystem::OnTime3()
{
	TrySetTimeSpeed(ETimeSpeedType::Ultra);
}

void UTimeSubsystem::HandleGameModeChanged(EGameModeState NewGameMode)
{
	if (NewGameMode == EGameModeState::Building)
	{
		SetTimePaused();
	}
	else if (NewGameMode == EGameModeState::Game)
	{
		SetTimeUnpaused();
	}
}

void UTimeSubsystem::SetTimeData(UTimeData* NewTimeData)
{
	TimeData = NewTimeData;
	GetCurrentSpeedValues();
}

bool UTimeSubsystem::TrySetTimeSpeed(ETimeSpeedType NewTimeSpeed)
{
	if (!bCanChangeTime)
		return false;

	SetTimeSpeed(NewTimeSpeed);
	return true;
}

void UTimeSubsystem::SetTimeSpeed(const ETimeSpeedType NewTimeSpeed)
{
	if (CurrentTimeSpeed == NewTimeSpeed)
		return;
	
	CurrentTimeSpeed = NewTimeSpeed;
	GetCurrentSpeedValues();
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), CurrentSpeedTimeDilation);
	OnTimeSpeedChanged.Broadcast(CurrentTimeSpeed);
}

void UTimeSubsystem::SetTimePausedWithEvent(const TSubclassOf<UMoralEvent> MoralEvent)
{
	SetTimePaused();
}

void UTimeSubsystem::SetTimePaused()
{
	SetTimeSpeed(ETimeSpeedType::Paused);
	bCanChangeTime = false;
}

void UTimeSubsystem::SetTimeUnpaused()
{
	SetTimeSpeed(ETimeSpeedType::Normal);
	bCanChangeTime = true;
}

void UTimeSubsystem::GetRandomMoralEventForDay(int Day) const
{
	for (const auto& [Day, MoralEventsDaysProbabilities] : TimeData->MoralEventDaysProbabilities)
	{
		if (Day == CurrentDay)
		{
			const TArray<FMoralEventProbabilitiesDataStruct>& Events = MoralEventsDaysProbabilities;

			if (Events.Num() == 0)
				return;
				
			float TotalProbability = 0.f;
			for (const auto& [MoralEvent, Probability] : Events)
			{
				TotalProbability += Probability;
			}

			if (TotalProbability <= 0.f)
				return;
				
			const float RandomValue = FMath::FRandRange(0.f, TotalProbability);

			float CumulativeProbability = 0.f;

			for (const auto& [MoralEvent, Probability] : Events)
			{
				CumulativeProbability += Probability;

				if (RandomValue <= CumulativeProbability)
				{
					OnMoralEventTime.Broadcast(MoralEvent);
					return;
				}
			}

			return;
		}
	}
}

void UTimeSubsystem::GetCurrentSpeedValues()
{
	for (const auto& [TimeSpeedType, GameSpeedMultiplier, GameTimeDilation] : TimeData->TimeData)
	{
		if (TimeSpeedType == CurrentTimeSpeed)
		{
			CurrentSpeedMultiplier = GameSpeedMultiplier;
			CurrentSpeedTimeDilation = GameTimeDilation;
			return;
		}
	}
}
