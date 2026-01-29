#pragma once

#include "CoreMinimal.h"
#include "TimeData.h"
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

	UFUNCTION()
	void SetTimeData(UTimeData* NewTimeData) { TimeData = NewTimeData; }
	
private:
	UPROPERTY()
	float CurrentMinutes = 0;

	UPROPERTY()
	EGameSpeedType CurrentGameSpeed = EGameSpeedType::Normal;
	
	UPROPERTY()
	UTimeData* TimeData = nullptr; // Get from game state
};
