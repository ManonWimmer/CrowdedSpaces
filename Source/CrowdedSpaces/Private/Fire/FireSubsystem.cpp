#include "Fire/FireSubsystem.h"

#include "IContentBrowserSingleton.h"
#include "IPropertyTable.h"
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

void UFireSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentTime += DeltaTime;

	if (CurrentTime > SpawnAfterTime)
	{
		SpreadFire();
		CurrentTime = 0.0f;
	}
}

void UFireSubsystem::StartFire()
{
	bIsInFire = true;

	SpawnFireAtRandomCell();
}

void UFireSubsystem::SpawnFireAtRandomCell()
{
	const auto RandomLocationWorldAndGrid = GetRandomSpawnLocation();
	
	AFire* SpawnedFire = GetWorld()->SpawnActor<AFire>(GameState->FireBP, RandomLocationWorldAndGrid.Key,FRotator::ZeroRotator);
	SpawnedFire->GridCoords = RandomLocationWorldAndGrid.Value;
	
	SpawnedFires.Add(SpawnedFire);
}

void UFireSubsystem::SpawnFireAtCell(const int Row, const int Column)
{
	FVector CellLocation = FVector::ZeroVector;
	const bool FoundCell = BuildSubsystem->GetCellWorldPos(CellLocation, Row, Column);
	
	if (!FoundCell)
		return;

	AFire* SpawnedFire = GetWorld()->SpawnActor<AFire>(GameState->FireBP, CellLocation, FRotator::ZeroRotator);
	SpawnedFire->GridCoords = FIntPoint(Row, Column);

	UE_LOG(LogTemp, Warning, TEXT("Spawn fire at %s"), *CellLocation.ToString());
	
	SpawnedFires.Add(SpawnedFire);
}

TPair<FVector, FIntPoint> UFireSubsystem::GetRandomSpawnLocation() const
{
	FVector RandomLocation = FVector::ZeroVector;

	if (!BuildSubsystem)
		return TPair<FVector, FIntPoint>(FVector::ZeroVector, FIntPoint(-1, -1));

	int RandomRow = 0;
	int RandomColumn = 0;

	const bool FoundRandomCell = BuildSubsystem->GetRandomCellWorldPos(RandomLocation, RandomRow,RandomColumn, true);

	if (!FoundRandomCell)
		return TPair<FVector, FIntPoint>(FVector::ZeroVector, FIntPoint(-1, -1));
	
	return TPair<FVector, FIntPoint>(RandomLocation, FIntPoint(RandomRow, RandomColumn));
}

bool UFireSubsystem::IsCellAlreadyOnFire(const FIntPoint IntPoint)
{
	for (auto Fire : SpawnedFires)
	{
		if (Fire.Get() == nullptr)
			continue;

		if (Fire->GridCoords == IntPoint)
			return true;
	}

	return false;
}

void UFireSubsystem::SpreadFire()
{
	if (SpawnedFires.Num() == 0)
		return;
	
	const AFire* Fire = SpawnedFires[FMath::RandRange(0, SpawnedFires.Num() - 1)];
	if (!Fire)
		return;

	const FIntPoint& C = Fire->GridCoords;
	
	TArray<FIntPoint> Neighbors =
	{
		{C.X + 1, C.Y},
		{C.X - 1, C.Y},
		{C.X, C.Y + 1},
		{C.X, C.Y - 1}
	};
	
	for (int i = 0; i < Neighbors.Num(); i++)
	{
		int j = FMath::RandRange(i, Neighbors.Num() - 1);
		Neighbors.Swap(i, j);
	}
	
	for (const FIntPoint& N : Neighbors)
	{
		if (!BuildSubsystem->CheckIsValidCell(N.X, N.Y))
			continue;

		if (IsCellAlreadyOnFire(N))
			continue;
		
		SpawnFireAtCell(N.X, N.Y);
		return;
	}
}

