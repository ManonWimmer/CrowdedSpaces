#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TrainingSubsystem.generated.h"

class UTrainingData;

UCLASS()
class CROWDEDSPACES_API UTrainingSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual TStatId GetStatId() const override;

	UFUNCTION()
	void SetTrainingData(UTrainingData* NewTrainingData) { TrainingData = NewTrainingData; }

private:
	UPROPERTY()
	TObjectPtr<UTrainingData> TrainingData{nullptr}; // Sent by game state
};
