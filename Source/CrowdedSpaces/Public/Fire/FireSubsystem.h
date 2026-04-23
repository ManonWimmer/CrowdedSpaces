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

public:
	void StartFire();
	void SpawnFireAtRandomCell();
	FVector GetRandomSpawnLocation();

private:
	bool bIsInFire = false;

	UPROPERTY()
	TArray<TObjectPtr<AFire>> SpawnedFires;

	UPROPERTY()
	TObjectPtr<ACrowdedGameState> GameState{nullptr};

	UPROPERTY()
	TObjectPtr<UBuildSubsystem> BuildSubsystem{nullptr};
};
