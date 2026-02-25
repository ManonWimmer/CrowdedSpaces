#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProductionType.h"
#include "Resources/ResourceComponent.h"
#include "ProductionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIsActiveChanged, bool, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProductionIntervalChanged, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResourcePerIntervalChanged, float, Value);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWDEDSPACES_API UProductionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UProductionComponent();

public:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category = "Production")
	EProductionType GetProductionType() const { return ProductionType; }

	UFUNCTION(BlueprintCallable, Category = "Production")
	void SetProductionType(const EProductionType NewProductionType) { ProductionType = NewProductionType; }
	
	UFUNCTION(BlueprintCallable, Category = "Production")
	float GetProductionInterval() const { return ProductionInterval; }

	UFUNCTION(BlueprintCallable, Category = "Production")
	void SetProductionInterval(const int NewProductionInterval) { ProductionInterval = NewProductionInterval; }

	UFUNCTION(BlueprintCallable, Category = "Production")
	float GetResourcePerInterval() const { return ResourcePerInterval; }

	UFUNCTION(BlueprintCallable, Category = "Production")
	void SetResourcePerInterval(const int NewResourcePerInterval) { ResourcePerInterval = NewResourcePerInterval; }

	UFUNCTION(BlueprintCallable, Category = "Production")
	void SetProductionMultiplier(const int Multiplier) { ProductionMultiplier = Multiplier; }
	
	UFUNCTION()
	void GenerateProduction() const;

	UFUNCTION()
	void StartProduction();

	UFUNCTION()
	void PauseProduction();

	UFUNCTION()
	void ResumeOrStartProduction();

	UFUNCTION()
	void RestartProduction();

	UPROPERTY(BlueprintAssignable)
	FOnIsActiveChanged OnIsActiveChanged;

	UPROPERTY(BlueprintAssignable)
	FOnProductionIntervalChanged OnProductionIntervalChanged;

	UPROPERTY(BlueprintAssignable)
	FOnResourcePerIntervalChanged OnResourcePerIntervalChanged;

private:
	UPROPERTY(EditAnywhere, Category="Production")
	EProductionType ProductionType = EProductionType::Money;

	UPROPERTY(EditAnywhere, Category="Production")
	float ProductionInterval = 1.0f;

	UPROPERTY(EditAnywhere, Category="Production")
	int32 ResourcePerInterval = 10;

	UPROPERTY(EditAnywhere, Category="Production")
	int ProductionMultiplier = 1;
	
	UPROPERTY()
	FTimerHandle ProductionTimerHandle;

	UPROPERTY()
	TObjectPtr<UResourceComponent> PlayerMoneyComponent;

	UPROPERTY()
	TObjectPtr<UResourceComponent> PlayerElectricityComponent;

	UPROPERTY()
	TObjectPtr<UResourceComponent> PlayerOxygenComponent;

	UPROPERTY()
	TObjectPtr<UResourceComponent> PlayerFoodComponent;

	bool bHasStartedProduction = false;
};
