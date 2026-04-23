#include "Fire/FireSubsystem.h"

#include "Fire/Fire.h"
#include "Game/CrowdedGameState.h"
#include "Build/BuildSubsystem.h"
#include "Grid/GridCell.h"

void UFireSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	const TObjectPtr<UWorld> World = InWorld.GetWorld();
	if (!World)
		return;

	GameState = World->GetGameState<ACrowdedGameState>();
	if (!GameState)
		return;

	BuildSubsystem = World->GetSubsystem<UBuildSubsystem>();
}

TStatId UFireSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UElectricitySubsystem, STATGROUP_Tickables);
}

void UFireSubsystem::StartFire()
{
	bIsInFire = true;

	SpawnFireAtRandomCell();
}

void UFireSubsystem::SpawnFireAtRandomCell()
{
	const FVector RandomLocation = GetRandomSpawnLocation();
	
	AFire* SpawnedFire = GetWorld()->SpawnActor<AFire>(GameState->FireBP, RandomLocation,
			FRotator::ZeroRotator);

	SpawnedFires.Add(SpawnedFire);
}

FVector UFireSubsystem::GetRandomSpawnLocation()
{
	FGridCell RandomCell = BuildSubsystem->GetRandomGridcell();

	if (!BuildSubsystem)
		return FVector::ZeroVector;

	
}

