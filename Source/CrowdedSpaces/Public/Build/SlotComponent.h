#pragma once

#include "CoreMinimal.h"
#include "SlotComponent.generated.h"


class ANPC;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWDEDSPACES_API USlotComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	USlotComponent();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsOccupied = false;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ANPC> OccupyingNPC;

	UPROPERTY()
	UMaterialInstanceDynamic* MID;

	bool bOccupied = false;

	void SetOccupied(bool bNewOccupied, ANPC* NPC = nullptr);
	bool IsFree() const { return !bIsOccupied; }

private:
	void UpdateVisual();
};
