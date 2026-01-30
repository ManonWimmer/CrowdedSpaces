#pragma once

#include "CoreMinimal.h"
#include "TimeData.h"
#include "TimeSpeedType.h"
#include "Subsystems/WorldSubsystem.h"
#include "TimeSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeChanged, float, NewMinutes);

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

	UFUNCTION(BlueprintCallable, Category = "Time")
	float GetCurrentMinutes() { return CurrentMinutes; }

	UFUNCTION(BlueprintCallable, Category = "Time")
	UTimeData* GetTimeData() const { return TimeData; }
	
	UFUNCTION()
	void SetTimeData(UTimeData* NewTimeData);

	UFUNCTION(BlueprintCallable, Category = "Time")
	void SetGameSpeed(ETimeSpeedType NewTimeSpeed);
	
	UFUNCTION()
	void GetCurrentSpeedValues();
	
private:
	UPROPERTY()
	float CurrentMinutes = 0;

	UPROPERTY()
	ETimeSpeedType CurrentTimeSpeed = ETimeSpeedType::Normal;

	UPROPERTY()
	float CurrentSpeedMultiplier = 0;

	UPROPERTY()
	float CurrentSpeedTimeDilation = 0;
	
	UPROPERTY()
	UTimeData* TimeData = nullptr; // Get from game state
};
