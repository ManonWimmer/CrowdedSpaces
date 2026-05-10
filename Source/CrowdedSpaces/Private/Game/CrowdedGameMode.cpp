#include "Game/CrowdedGameMode.h"

#include "Game/CrowdedGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Time/TimeSubsystem.h"

ACrowdedGameMode::ACrowdedGameMode()
{
}

void ACrowdedGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	SetGameMode(EGameModeState::Game);

	const TObjectPtr<UTimeSubsystem> TimeSubsystem = GetWorld()->GetSubsystem<UTimeSubsystem>();
	if (!TimeSubsystem)
		return;
	
	TimeSubsystem->OnDayChanged.AddDynamic(this, &ACrowdedGameMode::CheckEndGame);
	OnGameModeChanged.AddDynamic(TimeSubsystem, &UTimeSubsystem::HandleGameModeChanged);

	// Reset game instance
	UCrowdedGameInstance* GameInstance = GetGameInstance<UCrowdedGameInstance>();
	if (!GameInstance)
		return;
	
	GameInstance->ResetGameSettings();
}

void ACrowdedGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UTimeSubsystem* TimeSubsystem = GetWorld()->GetSubsystem<UTimeSubsystem>();
	if (!TimeSubsystem)
		return;
	
	TimeSubsystem->OnDayChanged.RemoveDynamic(this, &ACrowdedGameMode::CheckEndGame);
}

void ACrowdedGameMode::SetGameMode(const EGameModeState NewGameMode)
{
	if (CurrentGameMode == NewGameMode)
		return;

	CurrentGameMode = NewGameMode;
	
	OnGameModeChanged.Broadcast(NewGameMode);
}

void ACrowdedGameMode::CheckEndGame(const int NewDay)
{
	if (NewDay > MaxDaysToSurvive)
	{
		EndGame(true);
	}
}

void ACrowdedGameMode::EndGame(const bool bSurvived) const
{
	UCrowdedGameInstance* GameInstance = GetGameInstance<UCrowdedGameInstance>();
	if (!GameInstance)
		return;
	
	GameInstance->bLastGameSurvived = bSurvived;

	UGameplayStatics::OpenLevel(this, "LVL_EndScreen");
}

void ACrowdedGameMode::RegisterNPC(ANPC* NPC)
{
	if (!NPC)
		return;

	const bool bWasEmpty = AliveNPCs.Num() == 0;
	
	AliveNPCs.Add(NPC);

	OnNbrAliveNPCChanged.Broadcast();
	OnNPCRegistered.Broadcast(NPC, bWasEmpty);
}

void ACrowdedGameMode::UnregisterNPC(ANPC* NPC)
{
	if (!NPC || !AliveNPCs.Contains(NPC))
		return;
	
	AliveNPCs.Remove(NPC);

	if (GetNbrAliveNPCs() <= 0)
	{
		EndGame(false); 
	}
	else
	{
		OnNbrAliveNPCChanged.Broadcast();
		OnNPCUnregistered.Broadcast(NPC);
	}
}

