#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Build/GhostObject.h"
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
	void StartBuilding(UStaticMesh* Mesh);
	void PlaceObject() const;

private:
	UPROPERTY()
	AGhostObject* CurrentGhost;

	UPROPERTY()
	UStaticMesh* CurrentGhostMesh;

	UPROPERTY(EditAnywhere, Category="Test")
	UStaticMesh* DefaultMesh; // test direct sans sélection

	float SnapSize;

	void UpdateGhost() const;
	bool CanPlace(const FVector& Location, const FVector& Extent) const;
	bool GetCursorHit(FVector& OutHit) const;
};
