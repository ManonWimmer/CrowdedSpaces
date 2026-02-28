#pragma once

#include "CoreMinimal.h"
#include "Resources/ResourceComponent.h"
#include "GameFramework/PlayerState.h"
#include "CrowdedPlayerState.generated.h"

UCLASS()
class CROWDEDSPACES_API ACrowdedPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ACrowdedPlayerState();

	UFUNCTION(BlueprintCallable)
	UResourceComponent* GetMoneyComponent() const { return MoneyComponent; }
	
	UFUNCTION(BlueprintCallable)
	UResourceComponent* GetElectricityComponent() const { return ElectricityComponent; }

	UFUNCTION(BlueprintCallable)
	UResourceComponent* GetOxygenComponent() const { return OxygenComponent; }

	UFUNCTION(BlueprintCallable)
	UResourceComponent* GetFoodComponent() const { return FoodComponent; }

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UResourceComponent> MoneyComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UResourceComponent> ElectricityComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UResourceComponent> OxygenComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UResourceComponent> FoodComponent;
};
