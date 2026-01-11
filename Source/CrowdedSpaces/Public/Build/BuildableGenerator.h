#pragma once

#include "CoreMinimal.h"
#include "BuildableObject.h"
#include "Build/ProductionType.h"
#include "Resources/MoneyComponent.h"
#include "BuildableGenerator.generated.h"

UCLASS()
class CROWDEDSPACES_API ABuildableGenerator : public ABuildableObject
{
	GENERATED_BODY()

public:
	ABuildableGenerator();

	UPROPERTY(EditAnywhere, Category="Generator")
	EProductionType ProductionType = EProductionType::Money;
	
	UPROPERTY(EditAnywhere, Category="Generator")
	float ProductionInterval = 1.0f;

	UPROPERTY(EditAnywhere, Category="Generator")
	int32 ResourcePerInterval = 10;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	FTimerHandle ProductionTimerHandle;

private:
	UFUNCTION()
	void GenerateProduction() const;

	UFUNCTION()
	void StartProduction();

	UFUNCTION()
	void PauseProduction() const;

	UFUNCTION()
	void ResumeProduction() const;

	UFUNCTION()
	void RestartProduction();

	UPROPERTY()
	UMoneyComponent* MoneyComponent;
};
