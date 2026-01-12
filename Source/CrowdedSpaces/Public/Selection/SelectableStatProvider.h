#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SelectableStatProvider.generated.h"




UINTERFACE(BlueprintType)
class USelectableStatProvider : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnStatChanged,
	FName, StatId,
	float, NewValue
);

class CROWDEDSPACES_API ISelectableStatProvider
{
	GENERATED_BODY()

public:
	virtual FString GetStatDisplayName() const = 0;
	virtual float GetCurrentValue() const = 0;
	virtual FOnStatChanged& GetOnStatChanged() = 0;
};
