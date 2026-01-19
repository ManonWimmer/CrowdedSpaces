#include "Debug/Debug.h"

#include "Game/CrowdedGameMode.h"
#include "Kismet/GameplayStatics.h"
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
	if (ACrowdedGameMode* GameMode = Cast<ACrowdedGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		GameMode->SetGameMode(EGameModeState::Game);
	}
}

void ADebug::SetGameMode_Building() const
{
	if (ACrowdedGameMode* GameMode = Cast<ACrowdedGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		GameMode->SetGameMode(EGameModeState::Building);
	}
}

void ADebug::StartDebugEventData() const
{
	UMoralEventManager* MoralEventManager = GetWorld()->GetSubsystem<UMoralEventManager>();
	
	if (!MoralEventManager || !DebugEventData) return;

	MoralEventManager->StartNewEvent(DebugEventData);
}

