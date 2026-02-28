#pragma once

#include "CoreMinimal.h"
#include "Build/BuildableObject.h"
#include "Production/ProductionComponent.h"
#include "Production/ProductionUpgradeData.h"
#include "Selection/Selectable.h"
#include "BuildableGenerator.generated.h"

class UResourceComponent;

UCLASS()
class CROWDEDSPACES_API ABuildableGenerator : public ABuildableObject, public ISelectable
{
	GENERATED_BODY()

public:
	ABuildableGenerator();

	virtual bool StartUsingImplementation(UBTTask_UseBuildableObject* UseObjectTask) override;
	virtual bool StopUsingImplementation(UBTTask_UseBuildableObject* UseObjectTask) override;
	
	// Selectable
	virtual void OnSelected() override;
	virtual void OnDeselected() override;

	UFUNCTION(BlueprintCallable, Category = "Upgrade")
	UProductionComponent* GetProductionComponent() const { return ProductionComponent; }

	UFUNCTION(BlueprintCallable, Category = "Upgrade")
	void OnNextUpgrade();

	UFUNCTION(BlueprintCallable, Category = "Upgrade")
	bool GetHasNextUpgrade();
	
	UFUNCTION(BlueprintCallable, Category = "Upgrade")
	FUpgradeStruct GetNextUpgrade();

	UFUNCTION(BlueprintCallable, Category = "Upgrade")
	UResourceComponent* GetPlayerMoneyComponent() const { return PlayerMoneyComponent; }

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
private:
	// Production
	UPROPERTY(EditAnywhere)
	UProductionComponent* ProductionComponent = nullptr;

	// Upgrade
	UPROPERTY(EditAnywhere)
	UProductionUpgradeData* ProductionUpgradeData = nullptr;

	UPROPERTY();
	int CurrentUpgrade = 0;

	UPROPERTY()
	bool bHasNextUpgrade;
	
	UPROPERTY()
	FUpgradeStruct NextUpgrade;

	UPROPERTY()
	UResourceComponent* PlayerMoneyComponent;
};
