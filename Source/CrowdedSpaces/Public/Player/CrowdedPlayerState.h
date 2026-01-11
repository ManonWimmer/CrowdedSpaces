#pragma once

#include "CoreMinimal.h"
#include "Player/MoneyComponent.h"
#include "Player/ElectricityComponent.h"
#include "Player/OxygenComponent.h"
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

private:
	UPROPERTY(VisibleAnywhere)
	UMoneyComponent* MoneyComponent;

	UPROPERTY(VisibleAnywhere)
	UElectricityComponent* ElectricityComponent;

	UPROPERTY(VisibleAnywhere)
	UOxygenComponent* OxygenComponent;
};
