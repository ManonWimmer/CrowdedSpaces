#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ElectricitySubsystem.generated.h"

class UBuildableRegistrySubsystem;
class UBuildSubsystem;
class UResourceComponent;
class ACrowdedGameState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoomEnoughElectricityChanged, int, RoomId); // pas dans la room directement car c'est une struct
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoomActiveStateChanged, int, RoomId); 

UCLASS()
class CROWDEDSPACES_API UElectricitySubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

protected:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual TStatId GetStatId() const override;

public:
	UFUNCTION()
	void OnTimeChanged(float NewTime);

	UPROPERTY(BlueprintAssignable)
	FOnRoomEnoughElectricityChanged OnRoomEnoughElectricityChanged;

	UPROPERTY(BlueprintAssignable)
	FOnRoomActiveStateChanged OnRoomActiveStateChanged;

	UFUNCTION(BlueprintCallable)
	void ChangeRoomActiveState(const int RoomId) const;

	UFUNCTION(BlueprintCallable)
	float GetRoomLoseElectricityPerHourPerCell(const int RoomId) const;

private:
	UPROPERTY()
	float LastTime = 0.f;

	UPROPERTY()
	TObjectPtr<ACrowdedGameState> GameState = nullptr;

	UPROPERTY()
	TObjectPtr<UResourceComponent> ElectricityComponent = nullptr;

	UPROPERTY()
	TObjectPtr<UBuildSubsystem> BuildSubsystem = nullptr;
	
	UPROPERTY()
	TObjectPtr<UBuildableRegistrySubsystem> BuildableRegistrySubsystem = nullptr;
};
