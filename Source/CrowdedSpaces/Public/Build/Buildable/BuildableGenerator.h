#pragma once

#include "CoreMinimal.h"
#include "Build/BuildableObject.h"
#include "Production/ProductionComponent.h"
#include "Production/ProductionUpgradeData.h"
#include "Selection/Selectable.h"
#include "BuildableGenerator.generated.h"

class UMoneyComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNPCWorkingChanged, bool, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNPCComingToGeneratorChanged, bool, Value);

UCLASS()
class CROWDEDSPACES_API ABuildableGenerator : public ABuildableObject, public ISelectable
{
	GENERATED_BODY()

public:
	ABuildableGenerator();

	UFUNCTION(BlueprintCallable, Category = "Generator")
	bool HasNPCComing() const { return bHasNPCComing; }

	void SetNPCWorking(bool bWorking);
	void SetHasNPCComing(bool NewAvailable);
	
	// Selectable
	virtual void OnSelected() override;
	virtual void OnDeselected() override;
	
	virtual FString GetDisplayName() const override;
	virtual TObjectPtr<AActor> GetSelectableActor() override;

	UFUNCTION(BlueprintCallable, Category = "Upgrade")
	UProductionComponent* GetProductionComponent() const { return ProductionComponent; }

	UFUNCTION(BlueprintCallable, Category = "Upgrade")
	void OnNextUpgrade();

	UFUNCTION(BlueprintCallable, Category = "Upgrade")
	bool GetHasNextUpgrade();
	
	UFUNCTION(BlueprintCallable, Category = "Upgrade")
	FUpgradeStruct GetNextUpgrade();

	UFUNCTION(BlueprintCallable, Category = "Upgrade")
	UMoneyComponent* GetPlayerMoneyComponent() const { return PlayerMoneyComponent; }
	
	UFUNCTION(BlueprintCallable, Category = "Upgrade")
	bool HasNPCWorking() const { return bHasNPCWorking; }

	UPROPERTY(BlueprintAssignable)
	FOnNPCWorkingChanged OnNPCWorkingChanged;

	UPROPERTY(BlueprintAssignable)
	FOnNPCComingToGeneratorChanged OnNPCComingToGeneratorChanged;

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
	
	bool bHasNPCComing = false;
};
