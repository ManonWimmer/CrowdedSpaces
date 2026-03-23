#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ElectricitySubsystem.generated.h"

class UResourceComponent;
class ACrowdedGameState;

UCLASS()
class CROWDEDSPACES_API UElectricitySubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

protected:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual TStatId GetStatId() const override;

public:
	UFUNCTION()
	void OnTimeChanged(float NewTime);

private:
	UPROPERTY()
	float LastTime = 0.f;

	UPROPERTY()
	TObjectPtr<ACrowdedGameState> GameState = nullptr;

	UPROPERTY()
	TObjectPtr<UResourceComponent> ElectricityComponent = nullptr;
};
