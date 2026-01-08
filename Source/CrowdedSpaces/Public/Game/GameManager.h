#pragma once

#include "CoreMinimal.h"
#include "Game/GameModeState.h"
#include "GameFramework/GameMode.h"
#include "GameManager.generated.h"
UCLASS()
class CROWDEDSPACES_API AGameManager : public AGameMode
{
	GENERATED_BODY()

public:
	AGameManager();

	virtual void BeginPlay() override;
};
