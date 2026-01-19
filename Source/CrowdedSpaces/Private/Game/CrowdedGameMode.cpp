#include "Game/CrowdedGameMode.h"

ACrowdedGameMode::ACrowdedGameMode()
{
}

void ACrowdedGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	SetGameMode(EGameModeState::Game);
}

void ACrowdedGameMode::SetGameMode(EGameModeState NewGameMode)
{
	if (CurrentGameMode == NewGameMode)
		return;

	CurrentGameMode = NewGameMode;
	
	OnGameModeChanged.Broadcast(NewGameMode);
}

