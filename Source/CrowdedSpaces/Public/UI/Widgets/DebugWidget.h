#pragma once

#include "CoreMinimal.h"
#include "UI/CustomWidget.h"
#include "DebugWidget.generated.h"

class UResourceComponent;

UCLASS()
class CROWDEDSPACES_API UDebugWidget : public UCustomWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	virtual void Init_Implementation() override;
	virtual void Reset_Implementation() override;
	virtual void Setup_Implementation(AActor* SelectableActor) override;
	virtual void Unsetup_Implementation() override;

private:
	UFUNCTION(BlueprintCallable, Category = "DebugWidget")
	void AddMoney(const int Amount) const;

	UFUNCTION(BlueprintCallable, Category = "DebugWidget")
	void AddElectricity(const int Amount) const;

	UFUNCTION(BlueprintCallable, Category = "DebugWidget")
	void AddFood(const int Amount) const;

	UPROPERTY()
	TObjectPtr<UResourceComponent> MoneyComponent;

	UPROPERTY()
	TObjectPtr<UResourceComponent> ElectricityComponent;

	UPROPERTY()
	TObjectPtr<UResourceComponent> FoodComponent;

	UFUNCTION(BlueprintCallable, Category = "DebugWidget")
	void SpawnNPC();
};
