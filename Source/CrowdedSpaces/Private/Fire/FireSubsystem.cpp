#include "Fire/FireSubsystem.h"

void UFireSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
}

TStatId UFireSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UElectricitySubsystem, STATGROUP_Tickables);
}
