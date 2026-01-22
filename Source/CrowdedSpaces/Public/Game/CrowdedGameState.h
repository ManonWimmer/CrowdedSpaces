#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Build/BuildData.h"
#include "CrowdedGameState.generated.h"

UCLASS()
class CROWDEDSPACES_API ACrowdedGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Build")
	TArray<UBuildData*> BuildDataObjects;

	UPROPERTY(EditAnywhere, Category = "Build")
	float SnapSize = 100.f;
};
