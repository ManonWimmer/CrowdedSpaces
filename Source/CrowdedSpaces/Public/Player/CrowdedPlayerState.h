#pragma once

#include "CoreMinimal.h"
#include "Resources/MoneyComponent.h"
#include "Resources/ElectricityComponent.h"
#include "Resources/OxygenComponent.h"
#include "Resources/FoodComponent.h"
#include "GameFramework/PlayerState.h"
#include "CrowdedPlayerState.generated.h"

UCLASS()
class CROWDEDSPACES_API ACrowdedPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ACrowdedPlayerState();

	UFUNCTION(BlueprintCallable)
	UMoneyComponent* GetMoneyComponent() const { return MoneyComponent; }
	
	UFUNCTION(BlueprintCallable)
	UElectricityComponent* GetElectricityComponent() const { return ElectricityComponent; }

	UFUNCTION(BlueprintCallable)
	UOxygenComponent* GetOxygenComponent() const { return OxygenComponent; }

	UFUNCTION(BlueprintCallable)
	UFoodComponent* GetFoodComponent() const { return FoodComponent; }

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMoneyComponent> MoneyComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UElectricityComponent> ElectricityComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UOxygenComponent> OxygenComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UFoodComponent> FoodComponent;
};
