#pragma once

#include "CoreMinimal.h"
#include "Player/MoneyComponent.h"
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

private:
	UPROPERTY(VisibleAnywhere)
	UMoneyComponent* MoneyComponent;
};
