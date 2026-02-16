#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GhostObject.generated.h"

UCLASS()
class CROWDEDSPACES_API AGhostObject : public AActor
{
	GENERATED_BODY()

public:
	AGhostObject();

	void SetMesh(UStaticMesh* InMesh) const;
	
private:
	UPROPERTY()
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> Mesh;
};
