#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Build/GhostObject.h"
#include "Build/BuildData.h"
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
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void StartBuilding(UBuildData* BuildData);

	UFUNCTION(BlueprintCallable)
	void StopBuilding();
	
	void PlaceObject() const;

	UFUNCTION(BlueprintCallable)
	TArray<UBuildData*> GetBuildDataObjects() { return BuildDataObjects;}

private:
	UPROPERTY()
	AGhostObject* CurrentGhost;

	UPROPERTY()
	UStaticMesh* CurrentGhostMesh;

	UPROPERTY()
	UBuildData* CurrentBuildData = nullptr;

	UPROPERTY(EditAnywhere, Category = "Build")
	TArray<UBuildData*> BuildDataObjects;

	float SnapSize;

	void UpdateGhost() const;
	bool CanPlace(const FVector& Location, const FVector& Extent) const;
	bool GetCursorHit(FVector& OutHit) const;
};
