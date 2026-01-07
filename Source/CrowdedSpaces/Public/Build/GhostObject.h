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

	void SetMesh(UStaticMesh* InMesh);
	void SetValid(bool bIsValid);
	FVector GetMeshExtent() const { return MeshExtent; }
	
private:
	UPROPERTY()
	USceneComponent* RootComp;

	UPROPERTY()
	UStaticMeshComponent* Mesh;

	FVector MeshExtent = FVector(50,50,50);

	UPROPERTY(EditAnywhere)
	UMaterialInterface* GhostBaseMaterial;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMat;
};
