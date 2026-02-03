#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "GridActor.generated.h"

UCLASS()
class CROWDEDSPACES_API AGridActor : public AActor
{
	GENERATED_BODY()

public:
	AGridActor();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	void DrawLine(FVector Start, FVector End, float Thickness, TArray<FVector>& Vertices, TArray<int>& Triangles);
	float LineWidth() const; 
	float LineHeight() const;
	TObjectPtr<UMaterialInstanceDynamic> CreateMaterialInstance(FLinearColor Color, float Opacity);
	
	UPROPERTY(EditAnywhere, Category = "Grid")
	TObjectPtr<UProceduralMeshComponent> LinesProceduralMesh;

	UPROPERTY(EditAnywhere, Category = "Grid")
	TObjectPtr<UProceduralMeshComponent> CellsProceduralMesh;

	UPROPERTY(EditAnywhere, Category="Materials")
	TObjectPtr<UMaterialInterface> BaseMaterial;

	UPROPERTY(EditAnywhere, Category = "Grid")
	int Rows = 10;

	UPROPERTY(EditAnywhere, Category = "Grid")
	int Columns = 10;

	UPROPERTY(EditAnywhere, Category = "Grid")
	float CellSize = 100.f;

	UPROPERTY(EditAnywhere, Category = "Grid")
	float LineThickness = 10.f;

	UPROPERTY(EditAnywhere, Category = "Grid")
	FLinearColor LineColor;

	UPROPERTY(EditAnywhere, Category = "Grid")
	FLinearColor CellColor;

	UPROPERTY(EditAnywhere, Category = "Grid")
	float LineOpacity = 1.f;

	UPROPERTY(EditAnywhere, Category = "Grid")
	float CellOpacity = 0.25f;
};