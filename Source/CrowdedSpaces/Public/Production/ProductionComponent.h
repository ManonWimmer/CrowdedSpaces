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

	// Production
	UPROPERTY(EditAnywhere, Category="Production")
	EProductionType ProductionType = EProductionType::Money;

	UFUNCTION(BlueprintCallable, Category = "Production")
	EProductionType GetProductionType() const { return ProductionType; }
	
	UPROPERTY(EditAnywhere, Category="Production")
	float ProductionInterval = 1.0f;

	UFUNCTION(BlueprintCallable, Category = "Production")
	float GetProductionInterval() const { return ProductionInterval; }

	UPROPERTY(EditAnywhere, Category="Production")
	int32 ResourcePerInterval = 10;

	UFUNCTION(BlueprintCallable, Category = "Production")
	float GetResourcePerInterval() const { return ResourcePerInterval; }

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
