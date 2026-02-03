#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Build/BuildData.h"
#include "MoralEvent/MoralEvent.h"
#include "Time/TimeData.h"
#include "CrowdedGameState.generated.h"

UCLASS()
class CROWDEDSPACES_API ACrowdedGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	// Build
	UPROPERTY(EditAnywhere, Category = "Build")
	TArray<TObjectPtr<UBuildData>> BuildDataObjects;

	UPROPERTY(EditAnywhere, Category = "Build")
	float SnapSize = 100.f;

	// Time
	UPROPERTY(EditAnywhere, Category = "Time")
	TObjectPtr<UTimeData> TimeData;

	// Moral Events
	UPROPERTY(EditAnywhere, Category = "MoralEvent")
	TArray<TSubclassOf<UMoralEvent>> PossibleMoralEvents;
};
