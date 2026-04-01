#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ElectricitySubsystem.generated.h"

class UBuildableRegistrySubsystem;
class UBuildSubsystem;
class UResourceComponent;
class ACrowdedGameState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoomEnoughElectricityChanged, int, RoomId); // pas dans la room directement car c'est une struct

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

	UFUNCTION(BlueprintCallable)
	void ChangeRoomActiveState(int RoomId);

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
