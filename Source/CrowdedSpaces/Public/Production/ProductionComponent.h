#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProductionType.h"
#include "Resources/MoneyComponent.h"
#include "Resources/ElectricityComponent.h"
#include "Resources/OxygenComponent.h"
#include "Resources/FoodComponent.h"
#include "ProductionComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWDEDSPACES_API UProductionComponent : public UActorComponent, public ISelectableStatProvider
{
	GENERATED_BODY()

public:
	UProductionComponent();

public:
	virtual void BeginPlay() override;
	
	// Selectable
	UPROPERTY(BlueprintAssignable)
	FOnStatChanged OnStatChanged;
	
	virtual TArray<TPair<FString, FString>> GetCurrentValues() const override;
	virtual FOnStatChanged& GetOnStatChanged() override { return OnStatChanged; }

	// Production
	UPROPERTY(EditAnywhere, Category="Production")
	EProductionType ProductionType = EProductionType::Money;
	
	UPROPERTY(EditAnywhere, Category="Production")
	float ProductionInterval = 1.0f;

	UPROPERTY(EditAnywhere, Category="Production")
	int32 ResourcePerInterval = 10;

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

private:
	UPROPERTY()
	FTimerHandle ProductionTimerHandle;

	UPROPERTY()
	UMoneyComponent* PlayerMoneyComponent;

	UPROPERTY()
	UElectricityComponent* PlayerElectricityComponent;

	UPROPERTY()
	UOxygenComponent* PlayerOxygenComponent;

	UPROPERTY()
	UFoodComponent* PlayerFoodComponent;

	bool bHasStartedProduction = false;
};
