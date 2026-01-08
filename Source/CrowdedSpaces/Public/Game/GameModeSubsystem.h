#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Game/GameModeState.h"
#include "GameModeSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameModeChanged, EGameModeState, NewGameMode);

UCLASS()
class CROWDEDSPACES_API UGameModeSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	EGameModeState GetGameMode() const { return CurrentGameMode; }
	
	void SetGameMode(EGameModeState NewGameMode);

	UPROPERTY(BlueprintAssignable)
	FOnGameModeChanged OnGameModeChanged;

private:
	EGameModeState CurrentGameMode = EGameModeState::Building;
};
