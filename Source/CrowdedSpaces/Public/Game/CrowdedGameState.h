#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Build/BuildData.h"
#include "Build/BuildRoomData.h"
#include "MoralEvent/MoralEvent.h"
#include "Time/TimeData.h"
#include "Resources/ResourceComponent.h"
#include "CrowdedGameState.generated.h"

UCLASS()
class CROWDEDSPACES_API ACrowdedGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ACrowdedGameState();
	
	virtual void BeginPlay() override;

	// Build
	UPROPERTY(EditAnywhere, Category = "Build")
	TArray<TObjectPtr<UBuildData>> BuildDataObjects;

	UPROPERTY(EditAnywhere, Category = "Build")
	TArray<TObjectPtr<UBuildRoomData>> BuildDataRooms;

	UPROPERTY(EditAnywhere, Category = "Build")
	float SnapSize = 100.f;

	// Time
	UPROPERTY(EditAnywhere, Category = "Time")
	TObjectPtr<UTimeData> TimeData;

	// Moral Events
	UPROPERTY(EditAnywhere, Category = "MoralEvent")
	TArray<TSubclassOf<UMoralEvent>> PossibleMoralEvents;

	// Resources
	UFUNCTION(BlueprintCallable)
	UResourceComponent* GetResourceComponentByType(EResourceType Type) const;

	UFUNCTION(BlueprintCallable)
	int GetResourceByType(EResourceType Type) const;

	template <EResourceType Type>
	UResourceComponent* GetResourceComponent() const;

	template <EResourceType Type>
	int GetResource() const;

private:
	UPROPERTY()
	TMap<EResourceType, TObjectPtr<UResourceComponent>> ResourceMap;
};

template <EResourceType Type>
UResourceComponent* ACrowdedGameState::GetResourceComponent() const
{
	return GetResourceComponentByType(Type);
}

template <EResourceType Type>
int ACrowdedGameState::GetResource() const
{
	if (!GetResourceComponentByType(Type))
		return 0;
	else
		return GetResourceComponentByType(Type)->GetResource();
}
