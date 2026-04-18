#pragma once

#include "CoreMinimal.h"
#include "AI/NameList.h"
#include "GameFramework/GameStateBase.h"
#include "Build/BuildData.h"
#include "Build/BuildRoomData.h"
#include "MoralEvent/MoralEvent.h"
#include "UI/Widgets/World/ActionWidget.h"
#include "Time/TimeData.h"
#include "Resources/ResourceComponent.h"
#include "CrowdedGameState.generated.h"

class UStorageData;
class UTrainingData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameDataReady);

UCLASS()
class CROWDEDSPACES_API ACrowdedGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ACrowdedGameState();
	
	virtual void BeginPlay() override;
	
	void TryInitSubsystems();
	
	UPROPERTY(BlueprintAssignable)
	FOnGameDataReady OnGameDataReady;

	// Build
	UPROPERTY(EditAnywhere, Category = "Build", meta=(AllowPrivateAccess=true))
	TArray<TObjectPtr<UBuildData>> BuildDataObjects;

	UPROPERTY(EditAnywhere, Category = "Build", meta=(AllowPrivateAccess=true))
	TArray<TObjectPtr<UBuildRoomData>> BuildDataRooms;

	UPROPERTY(EditAnywhere, Category = "Build")
	float SnapSize = 100.f;

	// Time
	UPROPERTY(EditAnywhere, Category = "Time", meta=(AllowPrivateAccess=true))
	TObjectPtr<UTimeData> TimeData;

	// Moral Events
	UPROPERTY(EditAnywhere, Category = "MoralEvent", meta=(AllowPrivateAccess=true))
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

	// NPC Name
	UPROPERTY(EditAnywhere, Category = "NPC", meta=(AllowPrivateAccess=true))
	TObjectPtr<UNameList> NameData{nullptr};

	UPROPERTY()
	bool bHasInitSubsystems = false;

	// Storage
	UPROPERTY(EditAnywhere, Category = "Storage", meta=(AllowPrivateAccess=true))
	TObjectPtr<UStorageData> StorageData{nullptr};

	// Storage
	UPROPERTY(EditAnywhere, Category = "Training", meta=(AllowPrivateAccess=true))
	TObjectPtr<UTrainingData> TrainingData{nullptr};

	// Outline
	UPROPERTY(EditAnywhere, Category = "Outline", meta=(AllowPrivateAccess=true))
	TObjectPtr<UMaterialInterface> OutlineMaterial{nullptr};

	// Actions
	UPROPERTY(EditAnywhere, Category = "Actions", meta=(AllowPrivateAccess=true))
	TSubclassOf<UActionWidget> ActionWidgetClass;

private:
	UPROPERTY()
	TMap<EResourceType, TObjectPtr<UResourceComponent>> ResourceMap;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UResourceComponent> MoneyComponent{nullptr};

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UResourceComponent> ElectricityComponent{nullptr};

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UResourceComponent> FoodComponent{nullptr};
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
