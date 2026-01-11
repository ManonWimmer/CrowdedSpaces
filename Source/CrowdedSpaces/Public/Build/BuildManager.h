#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Build/GhostObject.h"
#include "Build/BuildData.h"
#include "UI/GameHUD.h"
#include "Player/MoneyComponent.h"
#include "BuildManager.generated.h"

UCLASS()
class CROWDEDSPACES_API ABuildManager : public AActor
{
	GENERATED_BODY()

public:
	ABuildManager();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void OnGameModeChanged(EGameModeState NewMode);
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void StartBuilding(UBuildData* BuildData);

	UFUNCTION(BlueprintCallable)
	void StopBuilding();

	UFUNCTION()
	void PlaceObject();

	UFUNCTION(BlueprintCallable)
	TArray<UBuildData*> GetBuildDataObjects() { return BuildDataObjects;}

private:
	UPROPERTY()
	AGhostObject* CurrentGhost;

	UPROPERTY()
	UBuildData* CurrentBuildData = nullptr;

	UPROPERTY(EditAnywhere, Category = "Build")
	TArray<UBuildData*> BuildDataObjects;

	UPROPERTY(EditAnywhere, Category = "Build")
	float SnapSize = 100.f;

	UFUNCTION()
	void UpdateGhost() const;

	UFUNCTION()
	bool CanPlace(const FVector& Location, const FVector& Extent) const;

	UFUNCTION()
	bool GetCursorHit(FVector& OutHit) const;

	UPROPERTY()
	AGameHUD* GameHUD;

	UPROPERTY()
	UMoneyComponent* MoneyComponent;
};
