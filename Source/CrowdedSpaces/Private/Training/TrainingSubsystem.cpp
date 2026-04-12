#include "Training/TrainingSubsystem.h"
#include "Training/TrainingData.h"

void UTrainingSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
}

TStatId UTrainingSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UBuildSubsystem, STATGROUP_Tickables);
}
