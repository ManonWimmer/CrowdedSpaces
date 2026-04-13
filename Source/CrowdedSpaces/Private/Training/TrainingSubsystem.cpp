#include "Training/TrainingSubsystem.h"
#include "Training/TrainingData.h"
#include "AI/NPC.h"

void UTrainingSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
}

TStatId UTrainingSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UBuildSubsystem, STATGROUP_Tickables);
}

void UTrainingSubsystem::OnTimeChanged(const float NewTime)
{
	if (TrainingNPCs.Num() == 0)
		return;

	const float DeltaTime = NewTime - LastTime;
	LastTime = NewTime;

	if (DeltaTime <= 0.f)
		return;

	const float AddExpThisFrame = (TrainingData->AddExpPerHour / 60.f) * DeltaTime;

	for (ANPC* TrainingNPC : TrainingNPCs)
	{
		TrainingNPC->AddTrainingExp(AddExpThisFrame);
	}
}

void UTrainingSubsystem::AddTrainingNPC(ANPC* NPC)
{
	TrainingNPCs.Add(NPC);
}

void UTrainingSubsystem::RemoveTrainingNPC(ANPC* NPC)
{
	if (TrainingNPCs.Contains(NPC))
		TrainingNPCs.Remove(NPC);
}
