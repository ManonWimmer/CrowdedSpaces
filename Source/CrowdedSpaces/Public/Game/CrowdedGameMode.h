#pragma once

#include "CoreMinimal.h"
#include "Game/GameModeState.h"
#include "GameFramework/GameMode.h"
#include "CrowdedGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameModeChanged, EGameModeState, NewGameMode);

UCLASS()
class CROWDEDSPACES_API ACrowdedGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ACrowdedGameMode();

	virtual void BeginPlay() override;
	
	EGameModeState GetGameMode() const { return CurrentGameMode; }

	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void SetGameMode(EGameModeState NewGameMode);

	UPROPERTY(BlueprintAssignable)
	FOnGameModeChanged OnGameModeChanged;

private:
	EGameModeState CurrentGameMode = EGameModeState::Game;
};
