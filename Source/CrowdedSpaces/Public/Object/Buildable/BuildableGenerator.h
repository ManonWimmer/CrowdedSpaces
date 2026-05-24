#pragma once

#include "CoreMinimal.h"
#include "Object/UsableObject.h"
#include "Production/ProductionComponent.h"
#include "Production/ProductionUpgradeData.h"
#include "Selection/Selectable.h"
#include "BuildableGenerator.generated.h"

class UResourceComponent;

UCLASS()
class CROWDEDSPACES_API ABuildableGenerator : public AUsableObject, public ISelectable
{
	GENERATED_BODY()

public:
	ABuildableGenerator();

	virtual bool StartUsingImplementation(ANPC* NPC) override;
	virtual bool StopUsingImplementation(ANPC* NPC) override;
	
	// Selectable
	virtual void OnSelected() override;
	virtual void OnDeselected() override;

	UFUNCTION(BlueprintCallable, Category = "Upgrade")
	UProductionComponent* GetProductionComponent() const { return ProductionComponent; }

	UFUNCTION(BlueprintCallable, Category = "Upgrade")
	EProductionType GetProductionType() const;

	UFUNCTION(BlueprintCallable, Category = "Upgrade")
	void OnNextUpgrade();

	UFUNCTION(BlueprintCallable, Category = "Upgrade")
	bool GetHasNextUpgrade();
	
	UFUNCTION(BlueprintCallable, Category = "Upgrade")
	FUpgradeStruct GetNextUpgrade();

	UFUNCTION(BlueprintCallable, Category = "Upgrade")
	UResourceComponent* GetPlayerMoneyComponent() const;

	// Actions
	virtual void InitActions() override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
private:
	// Production
	UPROPERTY(EditAnywhere)
	TObjectPtr<UProductionComponent> ProductionComponent{nullptr};

	// Upgrade
	UPROPERTY(EditAnywhere)
	TObjectPtr<UProductionUpgradeData> ProductionUpgradeData{nullptr};

	UPROPERTY();
	int CurrentUpgrade = 0;

	UPROPERTY()
	bool bHasNextUpgrade;
	
	UPROPERTY()
	FUpgradeStruct NextUpgrade;

	UPROPERTY()
	TObjectPtr<UResourceComponent> PlayerMoneyComponent{nullptr};
};
