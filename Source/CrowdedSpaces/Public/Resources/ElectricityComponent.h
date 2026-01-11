#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ElectricityComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnElectricityChanged, int32, NewAmount);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWDEDSPACES_API UElectricityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UElectricityComponent();
	
	UFUNCTION(BlueprintCallable)
	void AddElectricity(int Amount);

	UFUNCTION(BlueprintCallable)
	void RemoveElectricity(int Amount);

	UFUNCTION(BlueprintCallable)
	bool HasEnoughElectricity(int Amount);
	
	UFUNCTION(BlueprintCallable)
	int GetElectricity() {return Electricity; }

	UPROPERTY(BlueprintAssignable)
	FOnElectricityChanged OnElectricityChanged;
	
private:
	UPROPERTY(EditAnywhere)
	int32 Electricity = 0;
};
