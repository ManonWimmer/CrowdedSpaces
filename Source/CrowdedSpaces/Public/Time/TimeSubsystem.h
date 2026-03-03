#pragma once

#include "CoreMinimal.h"
#include "TimeData.h"
#include "TimeSpeedType.h"
#include "MoralEvent/MoralEvent.h"
#include "Subsystems/WorldSubsystem.h"
#include "TimeSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeChanged, float, NewMinutes);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDayChanged, int, NewDay);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoralEventTime, TSubclassOf<UMoralEvent>, MoralEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeSpeedChanged, ETimeSpeedType, NewTimeSpeed);

UCLASS()
class CROWDEDSPACES_API UTimeSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual TStatId GetStatId() const override;
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	
	UPROPERTY(BlueprintAssignable, Category = "Time")
	FOnTimeChanged OnTimeChanged;

	UPROPERTY(BlueprintAssignable, Category = "Time")
	FOnDayChanged OnDayChanged;

	UPROPERTY(BlueprintAssignable, Category = "Time")
	FOnMoralEventTime OnMoralEventTime;

	UPROPERTY(BlueprintAssignable, Category = "Time")
	FOnTimeSpeedChanged OnTimeSpeedChanged;

	UFUNCTION(BlueprintCallable, Category = "Time")
	float GetCurrentMinutes() const { return CurrentMinutes; }

	UFUNCTION(BlueprintCallable, Category = "Time")
	int GetCurrentDay() const { return CurrentDay; }

	UFUNCTION(BlueprintCallable, Category = "Time")
	UTimeData* GetTimeData() const { return TimeData; }
	
	UFUNCTION()
	void SetTimeData(UTimeData* NewTimeData);

	// Game speed
	UFUNCTION(BlueprintCallable, Category = "Time")
	void SetTimeSpeed(ETimeSpeedType NewTimeSpeed);
	
	UFUNCTION(BlueprintCallable, Category = "Time")
	void SetTimePaused(const TSubclassOf<UMoralEvent> MoralEvent = nullptr);

	void GetRandomMoralEventForDay(int Day) const;

	UFUNCTION(BlueprintCallable, Category = "Time")
	void SetTimeNormal();
	
	UFUNCTION()
	void GetCurrentSpeedValues();
	
private:
	UPROPERTY()
	float TotalMinutes = 0;
	
	UPROPERTY()
	float CurrentMinutes = 0;

	UPROPERTY()
	int CurrentDay = 1;

	UPROPERTY()
	int LastDayMoralEvent = 0;

	UPROPERTY()
	ETimeSpeedType CurrentTimeSpeed = ETimeSpeedType::Normal;

	UPROPERTY()
	float CurrentSpeedMultiplier = 0;

	UPROPERTY()
	float CurrentSpeedTimeDilation = 0;
	
	UPROPERTY()
	TObjectPtr<UTimeData> TimeData = nullptr; // Get from game state
};
