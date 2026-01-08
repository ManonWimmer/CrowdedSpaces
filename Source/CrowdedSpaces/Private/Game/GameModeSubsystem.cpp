#include "Game/GameModeSubsystem.h"

void UGameModeSubsystem::SetGameMode(EGameModeState NewGameMode)
{
	if (CurrentGameMode == NewGameMode)
		return;

	CurrentGameMode = NewGameMode;
	
	OnGameModeChanged.Broadcast(NewGameMode);
}
