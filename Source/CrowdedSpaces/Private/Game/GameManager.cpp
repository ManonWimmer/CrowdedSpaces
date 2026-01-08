#include "Game/GameManager.h"

#include "Game/GameModeSubsystem.h"

AGameManager::AGameManager()
{
}

void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	
	if (UGameModeSubsystem* Mode = GetWorld()->GetSubsystem<UGameModeSubsystem>())
	{
		Mode->SetGameMode(EGameModeState::Game);
	}
}

