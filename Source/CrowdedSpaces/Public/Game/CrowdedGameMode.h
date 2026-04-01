#pragma once

#include "CoreMinimal.h"
#include "Game/GameModeState.h"
#include "GameFramework/GameMode.h"
#include "CrowdedGameMode.generated.h"

class ANPC;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameModeChanged, EGameModeState, NewGameMode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNbrAliveNPCChanged);

UCLASS()
class CROWDEDSPACES_API ACrowdedGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ACrowdedGameMode();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	EGameModeState GetGameMode() const { return CurrentGameMode; }

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void SetGameMode(EGameModeState NewGameMode);

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void SetGameMode_Game() { SetGameMode(EGameModeState::Game); }

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void SetGameMode_Building() { SetGameMode(EGameModeState::Building); }

	UFUNCTION()
	void CheckEndGame(int NewDay);

	UFUNCTION()
	void EndGame(bool bSurvived) const;

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	int GetMaxDaysToSurvive() const { return MaxDaysToSurvive; }

	void RegisterNPC(ANPC* NPC);
	void UnregisterNPC(ANPC* NPC);

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	int GetNbrAliveNPCs() const { return AliveNPCCount; }
	
	UPROPERTY(BlueprintAssignable)
	FOnGameModeChanged OnGameModeChanged;

	UPROPERTY(BlueprintAssignable)
	FOnNbrAliveNPCChanged OnNbrAliveNPCChanged;

private:
	UPROPERTY(EditAnywhere, Category = "GameMode")
	int MaxDaysToSurvive = 10;

	UPROPERTY()
	EGameModeState CurrentGameMode = EGameModeState::Game;

	UPROPERTY()
	int AliveNPCCount = 0;
};
