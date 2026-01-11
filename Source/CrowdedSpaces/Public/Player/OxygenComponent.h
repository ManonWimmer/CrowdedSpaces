#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OxygenComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOxygenChanged, int32, NewAmount);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWDEDSPACES_API UOxygenComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UOxygenComponent();

	UFUNCTION(BlueprintCallable)
	void AddOxygen(int Amount);

	UFUNCTION(BlueprintCallable)
	void RemoveOxygen(int Amount);

	UFUNCTION(BlueprintCallable)
	bool HasEnoughOxygen(int Amount);

	UFUNCTION(BlueprintCallable)
	int GetOxygen() {return Oxygen; }

	UPROPERTY(BlueprintAssignable)
	FOnOxygenChanged OnOxygenChanged;
	
private:
	UPROPERTY(EditAnywhere)
	int32 Oxygen = 0;
};
