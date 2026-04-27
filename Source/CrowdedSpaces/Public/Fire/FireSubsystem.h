#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "FireSubsystem.generated.h"

class UBuildSubsystem;
class ACrowdedGameState;
class AFire;

UCLASS()
class CROWDEDSPACES_API UFireSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

protected:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual TStatId GetStatId() const override;
	virtual void Tick(float DeltaTime) override;

public:
	void StartFire();
	void CheckIsFireExtinguished();

	UFUNCTION()
	void OnFireExtinguished(AFire* Fire);
	void StopFire();
	void SpawnFireAtRandomCell();
	void SpawnFireAtCell(int Row, int Column);
	TPair<FVector, FIntPoint> GetRandomSpawnLocation() const;
	bool IsCellAlreadyOnFire(FIntPoint IntPoint);
	void SpreadFire();

private:
	bool bIsInFire = false;

	UPROPERTY()
	TArray<TObjectPtr<AFire>> SpawnedFires;

	UPROPERTY()
	TObjectPtr<ACrowdedGameState> GameState{nullptr};

	UPROPERTY()
	TObjectPtr<UBuildSubsystem> BuildSubsystem{nullptr};

	float SpawnAfterTime = 5.f;

	float CurrentTime = 0.f;
};
