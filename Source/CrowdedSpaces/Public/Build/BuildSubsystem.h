#pragma once

#include "CoreMinimal.h"
#include "BuildModeType.h"
#include "Game/GameModeState.h"
#include "Grid/GridRoomType.h"
#include "BuildSubsystem.generated.h"

struct FGridCell;
class AGameHUD;
class AGhostObject;
struct FGridRoom;
class UBuildRoomData;
class UBuildData;
class ABuildableObject;

USTRUCT()
struct FTMapArrayObjects
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<ABuildableObject*> Entries;
};

class AGridActor;
class UResourceComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeselected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoomDestroyed, int, RoomId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRoomCreated, int, RoomId, EGridRoomType, RoomType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRoomUpdated, int, RoomId, EGridRoomType, RoomType);

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
	void PlaceObject();

	UFUNCTION()
	void RemoveObject(const ABuildableObject* Object) const;
	
	UFUNCTION()
	void LeftClicked();

	UFUNCTION()
	void RightClicked();

	UFUNCTION(BlueprintCallable)
	void PlaceRoom();

	UFUNCTION()
	void SetBuildData(const TArray<UBuildData*>& NewBuildData) { BuildDataObjects = NewBuildData; }

	UFUNCTION()
	void SetBuildRoomData(const TArray<UBuildRoomData*>& NewBuildRoomData);

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

	UFUNCTION(BlueprintCallable)
	TMap<int, FGridRoom>& GetRooms();
	
	UFUNCTION(BlueprintCallable)
	void DestroyRoom(const int RoomId) const;
	
	FGridRoom* GetRoom(const int RoomId) const;

	UFUNCTION(BlueprintCallable)
	float GetRoomDestroyCost(const int RoomId) const;

	UFUNCTION(BlueprintCallable)
	int GetRoomCellsCount(const int RoomId) const;

	UFUNCTION(BlueprintCallable)
	void UnlockRoom(const EGridRoomType RoomType);

	UFUNCTION(BlueprintCallable)
	bool IsRoomUnlocked(const EGridRoomType RoomType) const;

	UPROPERTY(BlueprintAssignable)
	FOnDeselected OnDeselected; // To deselect ui

	UPROPERTY(BlueprintAssignable)
	FOnRoomDestroyed OnRoomDestroyed;

	UPROPERTY(BlueprintAssignable)
	FOnRoomCreated OnRoomCreated;

	UPROPERTY(BlueprintAssignable)
	FOnRoomUpdated OnRoomUpdated;

	static constexpr int InvalidRoomId = -1;

private:
	UPROPERTY()
	TObjectPtr<AGhostObject> CurrentGhost = nullptr;

	UPROPERTY()
	TObjectPtr<UBuildData> CurrentBuildData = nullptr;

	UPROPERTY()
	TObjectPtr<UBuildRoomData> CurrentBuildRoomData = nullptr;

	UPROPERTY()
	TArray<TObjectPtr<UBuildData>> BuildDataObjects; // Sent by game state

	UPROPERTY()
	TMap<EGridRoomType, bool> UnlockedRooms;

	UPROPERTY()
	TArray<TObjectPtr<UBuildRoomData>> BuildDataRooms; // Sent by game state
	
	UPROPERTY()
	float SnapSize = 100.f; // Sent by game state

	UFUNCTION()
	void UpdateGhost();

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

	int LastStartRow = 0;
	int LastStartCol = 0;

	UPROPERTY()
	int CurrentObjectGridRowsX = 1;

	UPROPERTY()
	int CurrentObjectGridColumnsY = 1;

	UPROPERTY()
	EGridRoomType CurrentObjectRoomType = EGridRoomType::Any;

	UPROPERTY()
	int CurrentObjectRoomId = InvalidRoomId; 
};
