#include "Debug/Debug.h"

#include "Game/GameModeSubsystem.h"

ADebug::ADebug()
{
}

void ADebug::BeginPlay()
{
	Super::BeginPlay();
}

void ADebug::Debug_SetGameMode_Game() const
{
	if (UGameModeSubsystem* Mode = GetWorld()->GetSubsystem<UGameModeSubsystem>())
	{
		Mode->SetGameMode(EGameModeState::Game);
	}
}

void ADebug::Debug_SetGameMode_Building() const
{
	if (UGameModeSubsystem* Mode = GetWorld()->GetSubsystem<UGameModeSubsystem>())
	{
		Mode->SetGameMode(EGameModeState::Building);
	}
}

