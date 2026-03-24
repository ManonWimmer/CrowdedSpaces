#pragma once

#include "CoreMinimal.h"
#include "BuildModeType.h"
#include "BuildRoomData.h"
#include "Subsystems/WorldSubsystem.h"
#include "Grid/GridRoomType.h"
#include "Grid/GridCell.h"
#include "Build/GhostObject.h"
#include "Build/BuildData.h"
#include "UI/GameHUD.h"
#include "BuildSubsystem.generated.h"

class AGridActor;
class UResourceComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeselected);

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
	void OnBuildModeSelected(EBuildModeType BuildMode);
	
	UFUNCTION(BlueprintCallable)
	void StartBuilding(UBuildData* BuildData);

	UFUNCTION(BlueprintCallable)
	void StartRoomBuilding(UBuildRoomData* BuildRoomData);

	UFUNCTION(BlueprintCallable)
	void StopBuilding();

	UFUNCTION()
	void PlaceObject() const;

	UFUNCTION()
	void RemoveObject(ABuildableObject* Object) const;
	
	UFUNCTION()
	void LeftClicked();

	UFUNCTION()
	void RightClicked();

	UFUNCTION(BlueprintCallable)
	void PlaceRoom();

	UFUNCTION()
	void SetBuildData(const TArray<UBuildData*>& NewBuildData) { BuildDataObjects = NewBuildData; }

	UFUNCTION()
	void SetBuildRoomData(const TArray<UBuildRoomData*>& NewBuildRoomData) { BuildDataRooms = NewBuildRoomData; }

	UFUNCTION()
	void SetSnapSize(const float NewSnapSize) { SnapSize = NewSnapSize; }

	UFUNCTION(BlueprintCallable)
	const TArray<UBuildData*>& GetBuildDataObjects() const  { return BuildDataObjects;}

	UFUNCTION(BlueprintCallable)
	const TArray<UBuildRoomData*>& GetBuildDataRooms() const { return BuildDataRooms;}

	UFUNCTION()
	void TryRotateBuildLeft();

	UFUNCTION()
	void TryRotateBuildRight();

	UFUNCTION()
	void ResetBuildRotation();

	UFUNCTION()
	void UpdateRotation();

	UFUNCTION()
	void GetObjectRotatedSize(int& OutX, int& OutY) const;

	UFUNCTION()
	void GetRoomRotatedSize(int& OutX, int& OutY) const;

	UFUNCTION()
	TMap<int, FGridRoom> GetRooms() const;

	UPROPERTY(BlueprintAssignable)
	FOnDeselected OnDeselected; // To deselect ui

private:
	UPROPERTY()
	TObjectPtr<AGhostObject> CurrentGhost = nullptr;

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
	TObjectPtr<UResourceComponent> MoneyComponent = nullptr;

	UPROPERTY()
	bool bTickEnabled = false;
	
	static constexpr float CursorLineTraceDistance = 10000.f;

	UPROPERTY()
	TObjectPtr<AGridActor> GridActor = nullptr;

	// Room
	EGridRoomType CurrentRoomType = EGridRoomType::Any;
	TArray<FGridCell*> SelectedRoomCells;
	bool bIsSelectingRoom = false;

	// Rotate
	int32 RotationIndex = 0;
	FRotator CurrentBuildRotation = FRotator(0, 0, 0);

	FVector MeshOffset;
};
