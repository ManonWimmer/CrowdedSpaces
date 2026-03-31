#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "StorageSubsystem.generated.h"

class UResourceComponent;

USTRUCT(BlueprintType)
struct FStorageRoomValues
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AddMaxMoney = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AddMaxFood = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AddMaxElectricity = 1000;
};

class UStorageData;

UCLASS()
class CROWDEDSPACES_API UStorageSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual TStatId GetStatId() const override;

	UFUNCTION()
	void OnRoomCreated(const int RoomId, const EGridRoomType RoomType);
	
	UFUNCTION()
	void AddStorageRoom(const int RoomId);

	UFUNCTION()
	void OnRoomDestroyed(const int RoomId);

	UFUNCTION()
	void SetStorageData(UStorageData* NewStorageData) {	StorageData = NewStorageData; }

	UFUNCTION(BlueprintCallable)
	FStorageRoomValues GetStorageValuesForCreatedRoom(const int RoomId);

private:
	UPROPERTY()
	TObjectPtr<UStorageData> StorageData = nullptr; // Sent by game state

	UPROPERTY()
	TMap<int, FStorageRoomValues> StorageRooms; // Room ID - Storage values

	UPROPERTY()
	TObjectPtr<UResourceComponent> MoneyComponent = nullptr;
	
	UPROPERTY()
	TObjectPtr<UResourceComponent> FoodComponent = nullptr;

	UPROPERTY()
	TObjectPtr<UResourceComponent> ElectricityComponent = nullptr;
};
