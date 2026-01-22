#include "Debug/Debug.h"

#include "Game/CrowdedGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "MoralEvent/MoralEventSubsystem.h"

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

void ADebug::StartDebugMoralEvent() const
{
	UMoralEventSubsystem* MoralEventSubsystem = GetWorld()->GetSubsystem<UMoralEventSubsystem>();
	
	if (!MoralEventSubsystem || !DebugEventData) return;

	MoralEventSubsystem->StartNewEvent(DebugEventData);
}

