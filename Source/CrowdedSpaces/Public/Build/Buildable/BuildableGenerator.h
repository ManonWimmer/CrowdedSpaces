#pragma once

#include "CoreMinimal.h"
#include "Build/BuildableObject.h"
#include "Production/ProductionComponent.h"
#include "Production/ProductionUpgradeData.h"
#include "Selection/Selectable.h"
#include "BuildableGenerator.generated.h"

UCLASS()
class CROWDEDSPACES_API ABuildableGenerator : public ABuildableObject, public ISelectable
{
	GENERATED_BODY()

public:
	ABuildableGenerator();

	void SetNPCWorking(bool bWorking);
	
	// Selectable
	virtual void OnSelected() override;
	virtual void OnDeselected() override;
	
	virtual FString GetDisplayName() const override;
	virtual AActor* GetSelectableActor() override;

	UProductionComponent* GetProductionComponent() const { return ProductionComponent; } 

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
private:
	UPROPERTY(EditAnywhere)
	UProductionComponent* ProductionComponent = nullptr;

	UPROPERTY(EditAnywhere)
	UProductionUpgradeData* ProductionUpgradeData = nullptr;
	
	UPROPERTY()
	bool bHasNPCWorking = false;
};
