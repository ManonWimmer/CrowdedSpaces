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

	UFUNCTION(BlueprintCallable)
	int GetResourceByType(EResourceType Type) const;

	template <EResourceType Type>
	UResourceComponent* GetResourceComponent() const;

	template <EResourceType Type>
	int GetResource() const;

private:
	UPROPERTY()
	TMap<EResourceType, TObjectPtr<UResourceComponent>> ResourceMap;
};

template <EResourceType Type>
UResourceComponent* ACrowdedPlayerState::GetResourceComponent() const
{
	return GetResourceComponentByType(Type);
}

template <EResourceType Type>
int ACrowdedPlayerState::GetResource() const
{
	if (!GetResourceComponentByType(Type))
		return 0;
	else
		return GetResourceComponentByType(Type)->GetResource();
}
