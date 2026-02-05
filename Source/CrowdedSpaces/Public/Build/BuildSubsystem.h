#pragma once

#include "CoreMinimal.h"
#include "BuildRoomData.h"
#include "Subsystems/WorldSubsystem.h"
#include "Grid/GridRoomType.h"
#include "Grid/GridCell.h"
#include "Build/GhostObject.h"
#include "Build/BuildData.h"
#include "UI/GameHUD.h"
#include "Resources/MoneyComponent.h"
#include "BuildSubsystem.generated.h"

class AGridActor;

UCLASS()
class CROWDEDSPACES_API UBuildSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual TStatId GetStatId() const override;

	UFUNCTION()
	void OnGameModeChanged(EGameModeState NewMode);
	
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	
	UFUNCTION(BlueprintCallable)
	void StartBuilding(UBuildData* BuildData);

	UFUNCTION(BlueprintCallable)
	void StartRoomBuilding(UBuildRoomData* BuildRoomData);

	UFUNCTION(BlueprintCallable)
	void StopBuilding();

	UFUNCTION()
	void PlaceObject();

	UFUNCTION(BlueprintCallable)
	void StartRoomSelection(EGridRoomType RoomType);

	UFUNCTION()
	void LeftClicked();

	UFUNCTION(BlueprintCallable)
	void ToggleRoomCell(int Row, int Column);

	UFUNCTION(BlueprintCallable)
	void PlaceRoom();

	UFUNCTION()
	void SetBuildData(const TArray<UBuildData*>& NewBuildData) { BuildDataObjects = NewBuildData; }

	UFUNCTION()
	void SetBuildRoomData(const TArray<UBuildRoomData*>& NewBuildRoomData) { BuildDataRooms = NewBuildRoomData; }

	UFUNCTION()
	void SetSnapSize(const float NewSnapSize) { SnapSize = NewSnapSize; }

	UFUNCTION(BlueprintCallable)
	TArray<UBuildData*> GetBuildDataObjects() { return BuildDataObjects;}

	UFUNCTION(BlueprintCallable)
	TArray<UBuildRoomData*> GetBuildDataRooms() { return BuildDataRooms;}
	

private:
	UPROPERTY()
	TObjectPtr<AGhostObject> CurrentGhost;

	UPROPERTY()
	TObjectPtr<UBuildData> CurrentBuildData = nullptr;

	UPROPERTY()
	TObjectPtr<UBuildRoomData> CurrentBuildRoomData = nullptr;

	UPROPERTY()
	TArray<TObjectPtr<UBuildData>> BuildDataObjects; // Send by game state

	UPROPERTY()
	TArray<TObjectPtr<UBuildRoomData>> BuildDataRooms; // Send by game state

	UPROPERTY()
	float SnapSize = 100.f; // Send by game state

	UFUNCTION()
	void UpdateGhost() const;

	UFUNCTION()
	void UpdateRoomSelection();

	UFUNCTION()
	bool GetCursorHit(FVector& OutHit) const;

	UPROPERTY()
	TObjectPtr<AGameHUD> GameHUD;

	UPROPERTY()
	TObjectPtr<UMoneyComponent> MoneyComponent;

	UPROPERTY()
	bool bTickEnabled = false;
	
	static constexpr float CursorLineTraceDistance = 10000.f;

	UPROPERTY()
	TObjectPtr<AGridActor> GridActor;

	// Room
	EGridRoomType CurrentRoomType = EGridRoomType::None;
	TArray<FGridCell*> SelectedRoomCells;
	bool bIsSelectingRoom = false;
};
