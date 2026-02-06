#pragma once

#include "CoreMinimal.h"
#include "Build/BuildableObject.h"
#include "Production/ProductionComponent.h"
#include "Production/ProductionUpgradeData.h"
#include "Selection/Selectable.h"
#include "BuildableGenerator.generated.h"

class UMoneyComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNPCWorkingChanged, bool, Value);

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
	virtual TObjectPtr<AActor> GetSelectableActor() override;

	UProductionComponent* GetProductionComponent() const { return ProductionComponent; }

	UFUNCTION(BlueprintCallable, Category = "Upgrade")
	void OnNextUpgrade();

	UFUNCTION(BlueprintCallable, Category = "Upgrade")
	bool GetHasNextUpgrade();
	
	UFUNCTION(BlueprintCallable, Category = "Upgrade")
	FUpgradeStruct GetNextUpgrade();

	UFUNCTION(BlueprintCallable, Category = "Upgrade")
	UMoneyComponent* GetPlayerMoneyComponent() { return PlayerMoneyComponent; }

	UFUNCTION(BlueprintCallable, Category = "Upgrade")
	UProductionComponent* GetProductionComponent() { return ProductionComponent; }

	UFUNCTION(BlueprintCallable, Category = "Upgrade")
	bool GetHasNPCWorking() { return bHasNPCWorking; }

	UPROPERTY(BlueprintAssignable)
	FOnNPCWorkingChanged OnNPCWorkingChanged;

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
	UMoneyComponent* PlayerMoneyComponent;

	// Work
	UPROPERTY()
	bool bHasNPCWorking = false;
};
