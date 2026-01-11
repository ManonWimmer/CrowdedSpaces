#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FoodComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFoodChanged, int32, NewAmount);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWDEDSPACES_API UFoodComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFoodComponent();

	UFUNCTION(BlueprintCallable)
	void AddFood(int Amount);

	UFUNCTION(BlueprintCallable)
	void RemoveFood(int Amount);

	UFUNCTION(BlueprintCallable)
	bool HasEnoughFood(int Amount);

	UFUNCTION(BlueprintCallable)
	int GetFood() {return Food; }

	UPROPERTY(BlueprintAssignable)
	FOnFoodChanged OnFoodChanged;
	
private:
	UPROPERTY(EditAnywhere)
	int32 Food = 0;
};
