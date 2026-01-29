#pragma once
using FStat = TPair<FString, FString>;

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
	FString, NewValue
);

class CROWDEDSPACES_API ISelectableStatProvider
{
	GENERATED_BODY()

public:
	virtual TArray<FStat> GetCurrentValues() const = 0; // DisplayName - Value
	virtual FOnStatChanged& GetOnStatChanged() = 0;
};
