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
	UResourceComponent* GetResourceComponentByType(EResourceType Type) const;

	template <EResourceType Type>
	UResourceComponent* GetResourceComponent() const;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UResourceComponent> MoneyComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UResourceComponent> ElectricityComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UResourceComponent> OxygenComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UResourceComponent> FoodComponent;

	UPROPERTY()
	TMap<EResourceType, TObjectPtr<UResourceComponent>> ResourceMap;
};

template <EResourceType Type>
UResourceComponent* ACrowdedPlayerState::GetResourceComponent() const
{
	return GetResourceComponentByType(Type);
}
