#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Build/BuildManager.h"
#include "Game/GameModeState.h"
#include "GameManager.generated.h"

UCLASS()
class CROWDEDSPACES_API AGameManager : public AActor
{
	GENERATED_BODY()

public:
	AGameManager();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EGameModeState CurrentMode;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ABuildManager* BuildManager;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPlayerClick();
	
	UFUNCTION()
	bool GetPlayerClickLocation() const;
};
