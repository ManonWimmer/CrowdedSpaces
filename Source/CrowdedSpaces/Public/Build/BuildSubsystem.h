#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Build/GhostObject.h"
#include "Grid/GridActor.h"
#include "Build/BuildData.h"
#include "UI/GameHUD.h"
#include "Resources/MoneyComponent.h"
#include "BuildSubsystem.generated.h"

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
	void StopBuilding();

	UFUNCTION()
	void PlaceObject();

	UFUNCTION()
	void SetBuildData(const TArray<UBuildData*>& NewBuildData) { BuildDataObjects = NewBuildData; }

	UFUNCTION()
	void SetSnapSize(const float NewSnapSize) { SnapSize = NewSnapSize; }

	UFUNCTION(BlueprintCallable)
	TArray<UBuildData*> GetBuildDataObjects() { return BuildDataObjects;}

private:
	UPROPERTY()
	TObjectPtr<AGhostObject> CurrentGhost;

	UPROPERTY()
	TObjectPtr<UBuildData> CurrentBuildData = nullptr;

	UPROPERTY()
	TArray<TObjectPtr<UBuildData>> BuildDataObjects; // Send by game state

	UPROPERTY()
	float SnapSize = 100.f; // Send by game state

	UFUNCTION()
	void UpdateGhost() const;

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
};
