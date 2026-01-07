#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildableObject.generated.h"

UCLASS()
class CROWDEDSPACES_API ABuildableObject : public AActor
{
	GENERATED_BODY()

public:
	ABuildableObject();

	UFUNCTION()
	void SetMesh(UStaticMesh* Mesh) const;

	UFUNCTION()
	FVector GetExtent() const;
	
private:
	UPROPERTY()
	UStaticMeshComponent* MeshComp;
};
