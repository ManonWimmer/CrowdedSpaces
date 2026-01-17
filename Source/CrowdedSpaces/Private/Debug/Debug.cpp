#include "Debug/Debug.h"

#include "Game/GameModeSubsystem.h"
#include "MoralEvent/MoralEventManager.h"

ADebug::ADebug()
{
}

void ADebug::BeginPlay()
{
	Super::BeginPlay();
}

void ADebug::SetGameMode_Game() const
{
	if (UGameModeSubsystem* Mode = GetWorld()->GetSubsystem<UGameModeSubsystem>())
	{
		Mode->SetGameMode(EGameModeState::Game);
	}
}

void ADebug::SetGameMode_Building() const
{
	if (UGameModeSubsystem* Mode = GetWorld()->GetSubsystem<UGameModeSubsystem>())
	{
		Mode->SetGameMode(EGameModeState::Building);
	}
}

void ADebug::StartDebugEventData() const
{
	UMoralEventManager* MoralEventManager = GetWorld()->GetSubsystem<UMoralEventManager>();
	
	if (!MoralEventManager || !DebugEventData) return;

	MoralEventManager->StartNewEvent(DebugEventData);
}

